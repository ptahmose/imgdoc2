// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

#include "documentMetadataReader.h"

using namespace std;
using namespace imgdoc2;

/*virtual*/imgdoc2::DocumentMetadataItem DocumentMetadataReader::GetItem(imgdoc2::dbIndex primary_key, imgdoc2::DocumentMetadataItemFlags flags)
{
    const auto statement = this->CreateStatementForRetrievingItem(flags);
    statement->BindInt64(1, primary_key);

    if (!this->GetDocument()->GetDatabase_connection()->StepStatement(statement.get()))
    {
        // this means that the tile with the specified index ('primary_key') was not found
        ostringstream ss;
        ss << "Request for reading a non-existing item (with pk=" << primary_key << ")";
        throw non_existing_item_exception(ss.str(), primary_key);
    }

    DocumentMetadataItem item = this->RetrieveDocumentMetadataItemFromStatement(statement, flags);
    return item;
}

/*virtual*/imgdoc2::DocumentMetadataItem DocumentMetadataReader::GetItemForPath(const std::string& path, imgdoc2::DocumentMetadataItemFlags flags)
{
    optional<imgdoc2::dbIndex> idx;
    const bool success = this->TryMapPathAndGetTerminalNode(path, &idx);
    if (success && idx.has_value())
    {
        // note: we require to have a valid node-id here, an "empty" optional (which would mean "empty path") is not valid in this method
        return this->GetItem(idx.value(), flags);
    }

    ostringstream string_stream;
    string_stream << "The path '" << path << "' does not exist.";
    throw invalid_path_exception(string_stream.str());
}

void DocumentMetadataReader::EnumerateItems(
  std::optional<imgdoc2::dbIndex> parent,
  bool recursive,
  DocumentMetadataItemFlags flags,
  const std::function<bool(imgdoc2::dbIndex, const DocumentMetadataItem& item)>& func)
{
    const auto statement = this->CreateStatementForEnumerateAllItemsWithAncestorAndDataBind(recursive, parent);

    while (this->GetDocument()->GetDatabase_connection()->StepStatement(statement.get()))
    {
        const imgdoc2::dbIndex index = statement->GetResultInt64(0);
        DocumentMetadataItem document_metadata_item = this->RetrieveDocumentMetadataItemFromStatement(statement, flags);
        const bool continue_operation = func(index, document_metadata_item);
        if (!continue_operation)
        {
            break;
        }
    }
}

void DocumentMetadataReader::EnumerateItemsForPath(
  const std::string& path,
  bool recursive,
  DocumentMetadataItemFlags flags,
  const std::function<bool(imgdoc2::dbIndex, const DocumentMetadataItem& item)>& func)
{
    optional<imgdoc2::dbIndex> idx;
    const bool success = this->TryMapPathAndGetTerminalNode(path, &idx);
    if (success)
    {
        return this->EnumerateItems(idx, recursive, flags, func);
    }

    throw runtime_error("DocumentMetadataReader::EnumerateItems");
}

std::shared_ptr<IDbStatement> DocumentMetadataReader::CreateStatementForRetrievingItem(imgdoc2::DocumentMetadataItemFlags flags)
{
    ostringstream string_stream;
    string_stream << "SELECT Pk, Name,TypeDiscriminator,ValueDouble,ValueInteger,ValueString FROM [" << "METADATA" << "] WHERE " <<
        "[" << "Pk" << "] = ?1;";
    auto statement = this->GetDocument()->GetDatabase_connection()->PrepareStatement(string_stream.str());
    return statement;
}

std::shared_ptr<IDbStatement> DocumentMetadataReader::CreateStatementForEnumerateAllItemsWithAncestorAndDataBind(bool recursive, std::optional<imgdoc2::dbIndex> parent)
{
    const bool parent_has_value = parent.has_value();
    ostringstream string_stream;

    if (recursive)
    {
        string_stream <<
            "WITH RECURSIVE cte AS(" <<
            "SELECT Pk, Name, AncestorId, TypeDiscriminator, ValueDouble, ValueInteger, ValueString  " <<
            "FROM METADATA ";

        if (parent_has_value)
        {
            string_stream << "WHERE AncestorId = ?1 ";
        }
        else
        {
            string_stream << "WHERE AncestorId IS NULL ";
        }

        string_stream <<
            "UNION ALL " <<
            "SELECT c.Pk, c.Name, c.AncestorId,c.TypeDiscriminator, c.ValueDouble, c.ValueInteger, c.ValueString " <<
            "FROM METADATA c " <<
            "JOIN cte ON c.AncestorId = cte.Pk " <<
            ") " <<
            "SELECT Pk, Name,TypeDiscriminator, ValueDouble, ValueInteger, ValueString FROM cte;";
    }
    else
    {
        string_stream <<
            "SELECT Pk, Name, TypeDiscriminator, ValueDouble, ValueInteger, ValueString FROM METADATA ";

        if (parent_has_value)
        {
            string_stream << "WHERE AncestorId = ?1 ";
        }
        else
        {
            string_stream << "WHERE AncestorId IS NULL ";
        }
    }

    auto statement = this->GetDocument()->GetDatabase_connection()->PrepareStatement(string_stream.str());
    if (parent_has_value)
    {
        statement->BindInt64(1, parent.value());
    }

    return statement;
}

imgdoc2::DocumentMetadataItem DocumentMetadataReader::RetrieveDocumentMetadataItemFromStatement(const std::shared_ptr<IDbStatement>& statement, imgdoc2::DocumentMetadataItemFlags flags)
{
    DocumentMetadataItem item;
    item.flags = flags;
    if ((flags & DocumentMetadataItemFlags::PrimaryKeyValid) == DocumentMetadataItemFlags::PrimaryKeyValid)
    {
        item.primary_key = statement->GetResultInt64(0);
    }

    if ((flags & DocumentMetadataItemFlags::NameValid) == DocumentMetadataItemFlags::NameValid)
    {
        item.name = statement->GetResultString(1);
    }

    if ((flags & DocumentMetadataItemFlags::DocumentMetadataTypeAndValueValid) == DocumentMetadataItemFlags::DocumentMetadataTypeAndValueValid)
    {
        const auto database_item_type_value = statement->GetResultInt32(2);
        switch (static_cast<DatabaseDataTypeValue>(database_item_type_value))
        {
            case DatabaseDataTypeValue::null:
                item.value = std::monostate();
                item.type = DocumentMetadataType::Null;
                break;
            case DatabaseDataTypeValue::int32:
                item.value = IDocumentMetadataWrite::metadata_item_variant(statement->GetResultInt32(4));
                item.type = DocumentMetadataType::Int32;
                break;
            case DatabaseDataTypeValue::doublefloat:
                item.value = IDocumentMetadataWrite::metadata_item_variant(statement->GetResultDouble(3));
                item.type = DocumentMetadataType::Double;
                break;
            case DatabaseDataTypeValue::utf8string:
                item.value = IDocumentMetadataWrite::metadata_item_variant(statement->GetResultString(5));
                item.type = DocumentMetadataType::Text;
                break;
            case DatabaseDataTypeValue::json:
                item.value = IDocumentMetadataWrite::metadata_item_variant(statement->GetResultString(5));
                item.type = DocumentMetadataType::Json;
                break;
            default:
                throw runtime_error("DocumentMetadataReader::GetItem: Unknown data type");
        }
    }
    return item;
}
