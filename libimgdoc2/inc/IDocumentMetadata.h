// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <functional>
#include <optional>
#include <type_traits>
#include "types.h"

namespace imgdoc2
{
    /// Values that represent the type of a document metadata item.
    enum class DocumentMetadataType : std::uint8_t
    {
        /// An enum constant representing the invalid option. This value is not legal for any call into the document metadata API.
        Invalid = 0,        

        /// An enum constant representing the default option.Default means that the type is determined from the value variant.
        /// The mapping is as follows: If the value variant is std::monostate, the type is set to Invalid. If the value variant is a string, the type is set to Text. 
        /// If the value variant is int32_t, the type is set to Int32. If the value variant is double, the type is set to Double. 
        Default,

        /// An enum constant representing the invalid option. This means that there is no value stored for this metadata item.
        Null,

        /// An enum constant representing the 'text' option. The value variant must contain a string.
        Text,
        Int32,
        Json,
        Double

    };

    /// Base interface for document metadata.
    class IDocumentMetadata
    {
    public:
        typedef std::variant<std::string, std::int32_t, double, std::monostate> metadata_item_variant;

        // no copy and no move (-> https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c21-if-you-define-or-delete-any-copy-move-or-destructor-function-define-or-delete-them-all )
        IDocumentMetadata() = default;
        IDocumentMetadata(const IDocumentMetadata&) = delete;             // copy constructor
        IDocumentMetadata& operator=(const IDocumentMetadata&) = delete;  // copy assignment
        IDocumentMetadata(IDocumentMetadata&&) = delete;                  // move constructor
        IDocumentMetadata& operator=(IDocumentMetadata&&) = delete;       // move assignment
    };

    enum class DocumentMetadataItemFlags : std::uint8_t
    {
        None = 0,
        PrimaryKeyValid = 1,
        NameValid = 2,
        DocumentMetadataTypeAndValueValid = 4,

        All = PrimaryKeyValid | NameValid | DocumentMetadataTypeAndValueValid
    };

    // -> https://stackoverflow.com/a/34220050/522591
    inline constexpr DocumentMetadataItemFlags operator|(DocumentMetadataItemFlags x, DocumentMetadataItemFlags y)
    {
        return static_cast<DocumentMetadataItemFlags>(static_cast<std::underlying_type_t<DocumentMetadataItemFlags>>(x) | static_cast<std::underlying_type_t<DocumentMetadataItemFlags>>(y));
    }

    inline constexpr DocumentMetadataItemFlags operator&(DocumentMetadataItemFlags x, DocumentMetadataItemFlags y)
    {
        return static_cast<DocumentMetadataItemFlags>(static_cast<std::underlying_type_t<DocumentMetadataItemFlags>>(x) & static_cast<std::underlying_type_t<DocumentMetadataItemFlags>>(y));
    }

    inline DocumentMetadataItemFlags& operator|=(DocumentMetadataItemFlags& x, DocumentMetadataItemFlags y)
    {
        x = x | y;
        return x;
    }

    struct DocumentMetadataItem
    {
        DocumentMetadataItemFlags flags{ DocumentMetadataItemFlags::None };
        imgdoc2::dbIndex primary_key{ 0 };
        std::string name;
        DocumentMetadataType type{ DocumentMetadataType::Invalid };
        IDocumentMetadata::metadata_item_variant value;
    };

    /// The interface for read-only access to document metadata.
    class IDocumentMetadataRead : public IDocumentMetadata
    {
    public:
        virtual ~IDocumentMetadataRead() = default;

        /// Get the item identified by the specified key. The argument 'flags' specifies which pieces of information should be retrieved.
        /// Only the information specified in the flags can be expected to be valid in the returned DocumentMetadataItem.
        /// If the item does not exist, an exception of type imgdoc2::non_existing_item_exception is thrown.
        ///
        /// \param  primary_key     The key of the metadata item to be retrieved.
        /// \param  flags           The flags.
        ///
        /// \returns    The item.
        virtual imgdoc2::DocumentMetadataItem GetItem(imgdoc2::dbIndex primary_key, DocumentMetadataItemFlags flags) = 0;

        virtual imgdoc2::DocumentMetadataItem GetItemForPath(const std::string& path, imgdoc2::DocumentMetadataItemFlags flags) = 0;
        virtual void EnumerateItems(
            std::optional<imgdoc2::dbIndex> parent,
            bool recursive,
            DocumentMetadataItemFlags flags,
            const std::function<bool(imgdoc2::dbIndex, const DocumentMetadataItem& item)>& func) = 0;
        virtual void EnumerateItemsForPath(
            const std::string& path,
            bool recursive,
            DocumentMetadataItemFlags flags,
            const std::function<bool(imgdoc2::dbIndex, const DocumentMetadataItem& item)>& func) = 0;

        // no copy and no move (-> https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c21-if-you-define-or-delete-any-copy-move-or-destructor-function-define-or-delete-them-all )
        IDocumentMetadataRead() = default;
        IDocumentMetadataRead(const IDocumentMetadataRead&) = delete;             // copy constructor
        IDocumentMetadataRead& operator=(const IDocumentMetadataRead&) = delete;  // copy assignment
        IDocumentMetadataRead(IDocumentMetadataRead&&) = delete;                  // move constructor
        IDocumentMetadataRead& operator=(IDocumentMetadataRead&&) = delete;       // move assignment
    };

    /// The interface for write access to document metadata.
    class IDocumentMetadataWrite : public IDocumentMetadata
    {
    public:
        virtual ~IDocumentMetadataWrite() = default;

        /// This method updates or creates a node with the name as specified in the parameter 'name'
        /// as a child of the node specified by 'parent'. If 'create_node_if_not_exists' is true, the
        /// node with the name 'name' is created if it does not exist. If 'create_node_if_not_exists' is
        /// false, the mode is not created if it does not exist. In this case, the method throws an
        /// 'TODO: node_does_not_exist' exception.
        ///
        /// \param  parent                      The parent node. If this is std::nullopt, it identifies the root node.
        /// \param  create_node_if_not_exists   If true, the node is created if it does not exist.
        /// \param  name                        The name of the node to be updated or created.
        /// \param  type                        The type of the node.
        /// \param  value                       The value of the node.
        ///
        /// \returns    The primary_key of the updated or created node.
        virtual imgdoc2::dbIndex UpdateOrCreateItem(
                    std::optional<imgdoc2::dbIndex> parent,
                    bool create_node_if_not_exists,
                    const std::string& name,
                    DocumentMetadataType type,
                    const IDocumentMetadata::metadata_item_variant& value) = 0;

        /// Deletes the item specified by 'primary_key'. If 'recursively' is true, all child nodes are
        /// also deleted. If 'recursively' is false, the node is only deleted if it has no child nodes.
        /// The method returns the number of deleted nodes - it does not throw an exception if the
        /// primary key does not exist or if the node has child nodes and 'recursively' is false.
        /// 
        /// \param  primary_key Key of the node to be deleted. If this the optional has no value, this means "the root".
        /// \param  recursively True if all child nodes should be deleted, false if only the node itself should be deleted.
        ///
        /// \returns    The number of deleted nodes as a result by this call.
        virtual std::uint64_t DeleteItem(
                    std::optional<imgdoc2::dbIndex> primary_key,
                    bool recursively) = 0;
        virtual std::uint64_t DeleteItemForPath(
                   const std::string& path,
                   bool recursively) = 0;
        virtual imgdoc2::dbIndex UpdateOrCreateItemForPath(
                    bool create_path_if_not_exists,
                    bool create_node_if_not_exists,
                    const std::string& path,
                    DocumentMetadataType type,
                    const IDocumentMetadata::metadata_item_variant& value) = 0;

        // no copy and no move (-> https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c21-if-you-define-or-delete-any-copy-move-or-destructor-function-define-or-delete-them-all )
        IDocumentMetadataWrite() = default;
        IDocumentMetadataWrite(const IDocumentMetadataWrite&) = delete;             // copy constructor
        IDocumentMetadataWrite& operator=(const IDocumentMetadataWrite&) = delete;  // copy assignment
        IDocumentMetadataWrite(IDocumentMetadataWrite&&) = delete;                  // move constructor
        IDocumentMetadataWrite& operator=(IDocumentMetadataWrite&&) = delete;       // move assignment
    };
}
