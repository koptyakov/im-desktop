#pragma once

#include "../../../namespaces.h"
#include "../../../types/message.h"

namespace Ui
{
    enum class MediaType;
    using highlightsV = std::vector<QString>;
}

UI_COMPLEX_MESSAGE_NS_BEGIN

enum class BlockSelectionType;
class ComplexMessageItem;
class IItemBlockLayout;

enum class PinPlaceholderType
{
    None,
    Link,
    Image,
    Video,
    Filesharing,
    FilesharingImage,
    FilesharingVideo,
    Sticker
};

class IItemBlock
{
public:
    enum MenuFlags
    {
        MenuFlagNone = 0,

        MenuFlagLinkCopyable = (1 << 0),
        MenuFlagFileCopyable = (1 << 1),
        MenuFlagOpenInBrowser = (1 << 2),
        MenuFlagCopyable = (1 << 3),
        MenuFlagOpenFolder = (1 << 4),
        MenuFlagRevokeVote = (1 << 5),
        MenuFlagStopPoll = (1 << 6),
        MenuFlagSpellItems = (1 << 7),
    };

    enum class ContentType
    {
        Other = 0,
        Text = 1,
        FileSharing = 2,
        Link = 3,
        Quote = 4,
        Sticker = 5,
        DebugText = 6,
        Poll = 7
    };

    virtual ~IItemBlock() = 0;

    virtual QSize blockSizeForMaxWidth(const int32_t maxWidth) = 0;

    virtual void clearSelection() = 0;

    virtual void deleteLater() = 0;

    virtual bool drag() = 0;

    virtual QString formatRecentsText() const = 0;

    virtual Ui::MediaType getMediaType() const = 0;

    virtual bool isDraggable() const = 0;

    virtual bool isSharingEnabled() const = 0;

    virtual bool containsSharingBlock() const { return false; }

    virtual bool standaloneText() const = 0;

    virtual void onActivityChanged(const bool isActive) = 0;

    virtual void onVisibilityChanged(const bool isVisible) = 0;

    virtual void onSelectionStateChanged(const bool isSelected) {};

    virtual void onDistanceToViewportChanged(const QRect& _widgetAbsGeometry, const QRect& _viewportVisibilityAbsRect) = 0;

    virtual QRect setBlockGeometry(const QRect &ltr) = 0;

    virtual MenuFlags getMenuFlags(QPoint p = {}) const = 0;

    virtual ComplexMessageItem* getParentComplexMessage() const = 0;

    enum class TextDestination
    {
        selection,
        quote
    };
    virtual QString getSelectedText(const bool _isFullSelect = false, const TextDestination _dest = TextDestination::selection) const = 0;

    virtual QString getSourceText() const = 0;

    virtual QString getTextInstantEdit() const = 0;

    virtual QString getPlaceholderText() const = 0;

    virtual const QString& getLink() const = 0;

    virtual QString getTextForCopy() const = 0;

    virtual bool isBubbleRequired() const = 0;

    virtual bool isMarginRequired() const = 0;

    virtual bool isSelected() const = 0;

    virtual bool isAllSelected() const = 0;

    virtual bool onMenuItemTriggered(const QVariantMap &params) = 0;

    virtual void selectByPos(const QPoint& from, const QPoint& to, bool topToBottom) = 0;

    virtual void selectAll() = 0;

    virtual void setSelected(bool _selected) = 0;

    virtual bool replaceBlockWithSourceText(IItemBlock* /*block*/) { return false; }

    virtual bool removeBlock(IItemBlock* /*block*/) { return false; }

    virtual bool isSimple() const { return true; }

    virtual bool updateFriendly(const QString& _aimId, const QString& _friendly) = 0;

    virtual Data::Quote getQuote() const;

    virtual bool needFormatQuote() const { return true; }

    virtual IItemBlock* findBlockUnder(const QPoint &pos) const { return nullptr; }

    virtual ContentType getContentType() const { return ContentType::Other; }

    virtual QString getTrimmedText() const { return QString(); }

    virtual void setQuoteSelection() = 0;

    virtual void hideBlock() = 0;

    virtual bool isHasLinkInMessage() const = 0;

    virtual int getMaxWidth() const { return -1; }

    virtual QPoint getShareButtonPos(const bool _isBubbleRequired, const QRect& _bubbleRect) const = 0;

    virtual bool pressed(const QPoint& _p) { return false; }

    virtual bool clicked(const QPoint& _p) { return true; }

    virtual void releaseSelection() { };

    virtual void doubleClicked(const QPoint& _p, std::function<void(bool)> _callback = std::function<void(bool)>()) { if (_callback) _callback(true); };

    virtual QString linkAtPos(const QPoint& pos) const { return QString(); }

    [[nodiscard]] virtual std::optional<QString> getWordAt(QPoint) const { return {}; }
    [[nodiscard]] virtual bool replaceWordAt(const QString&, const QString&, QPoint) { return false; }

    virtual int desiredWidth(int _width = 0) const { return 0; }

    virtual bool isSizeLimited() const { return false; }

    virtual PinPlaceholderType getPinPlaceholder() const { return PinPlaceholderType::None; }

    virtual void requestPinPreview() { }

    virtual bool isLinkMediaPreview() const { return false; }
    virtual bool isSmallPreview() const { return false; }

    virtual bool canStretchWithSender() const { return true; }

    virtual QString getSenderAimid() const = 0;

    virtual Data::StickerId getStickerId() const { return {}; }

    virtual void resetClipping() {}

    virtual void cancelRequests() {}

    virtual bool isNeedCheckTimeShift() const { return false; }

    virtual void shiftHorizontally(const int _shift) {}

    virtual void setText(const QString& _text) {}

    virtual void startSpellChecking() {}

    virtual void updateStyle() = 0;
    virtual void updateFonts() = 0;

    virtual void highlight(const highlightsV& _hl) {}
    virtual void removeHighlight() {}

    virtual Data::FilesPlaceholderMap getFilePlaceholders() = 0;

    virtual bool isPreviewable() const { return false; }

    virtual void updateWith(IItemBlock* _other) {}

    virtual bool needStretchToOthers() const = 0;

    virtual void stretchToWidth(const int _width) = 0;

    virtual QRect getBlockGeometry() const = 0;

    virtual bool hasLeadLines() const = 0;

    virtual void markDirty() = 0;

    virtual bool managesTime() const { return false; }

    virtual bool needStretchByShift() const { return false; }

    virtual void resizeBlock(const QSize& _size) {}

    virtual void onBlockSizeChanged(const QSize& _size) {}

    virtual bool hasSourceText() const { return true; }

    virtual int effectiveBlockWidth() const { return 0; }

    virtual void setSpellErrorsVisible(bool _visible) {}

protected:
    virtual IItemBlockLayout* getBlockLayout() const = 0;
};

using IItemBlocksVec = std::vector<IItemBlock*>;

UI_COMPLEX_MESSAGE_NS_END
