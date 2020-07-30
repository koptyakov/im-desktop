#pragma once
#include "CommonUI.h"
#include "VoipProxy.h"
#include "../controls/ClickWidget.h"
#include "media/permissions/MediaCapturePermissions.h"

namespace voip_manager
{
    struct ContactEx;
    struct Contact;
}

namespace Previewer
{
    class CustomMenu;
}

namespace Ui
{
    struct UIEffects;
    class Toast;

    namespace TextRendering
    {
        class TextUnit;
    }

    namespace VideoPanelParts
    {
        class PanelButton : public ClickableWidget
        {
            Q_OBJECT

        public:
            enum class ButtonStyle
            {
                Normal,
                Transparent,
                Red
            };

            PanelButton(QWidget* _parent, const QString& _text, const QString& _iconName, ButtonStyle _style = ButtonStyle::Normal);

            void updateStyle(ButtonStyle _style, const QString& _iconName = {});

        protected:
            void paintEvent(QPaintEvent* _event) override;

        private:
            QString iconName_;
            QPixmap icon_;
            QColor circleNormal_;
            QColor circleHovered_;
            QColor circlePressed_;
            QColor textNormal_;
            QColor textHovered_;
            QColor textPressed_;

            std::unique_ptr<TextRendering::TextUnit> textUnit_;
        };

        class ScreenBorderLine : public QWidget
        {
            Q_OBJECT

        public:
            enum class LinePosition
            {
                Left,
                Top,
                Right,
                Bottom
            };

            explicit ScreenBorderLine(LinePosition _position, const Qt::WindowFlags& _wFlags, const QColor& _color);
            ~ScreenBorderLine();

            void updateForGeometry(const QRect& _rect);

        protected:
            void paintEvent(QPaintEvent* _event) override;

        private:
            bool isOverlapped() const;

            LinePosition linePosition_;
            uint lineWidth_;
            QColor bgColor_;

            QTimer* checkOverlappedTimer_;
        };

        class ShareScreenFrame : public QObject
        {
            Q_OBJECT

        Q_SIGNALS:
            void stopScreenSharing(QPrivateSignal) const;

        public:
            explicit ShareScreenFrame(std::string_view _uid, const QColor& _color = Qt::red, qreal _opacity = 0.5);
            ~ShareScreenFrame();

        private Q_SLOTS:
            void screenGeometryChanged(const QRect& _rect);

        private:
            QPointer<ClickableTextWidget> buttonStopScreenSharing_;
            std::vector<QPointer<ScreenBorderLine>> borderLines_;
        };
    }

    class VideoPanel : public BaseBottomVideoPanel
    {
        Q_OBJECT

        Q_SIGNALS:
        void onMouseEnter();
        void onMouseLeave();
        void onkeyEscPressed();
        // Update conferention layout.
        void updateConferenceMode(voip_manager::VideoLayout _layout);
        void onShowMaskPanel();
        void onCameraClickOn();
        void onShareScreenClickOn();
        void onGoToChatButton();
        void onMicrophoneClick();
        void onSpeakerClick();
        void needShowScreenPermissionsPopup(media::permissions::DeviceType type);
        void addUserToConference();
        void parentWindowWillDeminiaturize();
        void parentWindowDidDeminiaturize();
        void closeActiveMask();
        void inviteVCSUrl(const QString& _url) const;

    private Q_SLOTS:
        void onHangUpButtonClicked();
        void onVideoOnOffClicked();

        void onClickGoChat();

        void onVoipMediaLocalVideo(bool _enabled);

        void onShareScreen();

        void onVoipVideoDeviceSelected(const voip_proxy::device_desc& _desc);

        void onCaptureAudioOnOffClicked();
        void onVoipMediaLocalAudio(bool _enabled);

        void onSpeakerOnOffClicked();
        void onVoipVolumeChanged(const std::string& _deviceType, int _vol);

        void onMoreButtonClicked();
        void onChangeConferenceMode();
        void onClickSettings();

        void onClickOpenMasks();
        void onVoipCallNameChanged(const voip_manager::ContactsList& _contacts);

        void onClickCopyVCSLink();
        void onClickInviteVCS();

    public:
        VideoPanel(QWidget* _parent, QWidget* _container);
        ~VideoPanel();

        void setVisible(bool _visible) override;

        void updatePosition(const QWidget& _parent) override;

        void fadeIn(unsigned int _kAnimationDefDuration) override;
        void fadeOut(unsigned int _kAnimationDefDuration) override;
        bool isFadedIn() override;

        bool isActiveWindow();
        void setContacts(std::vector<voip_manager::Contact> _contacts, bool _activeCall);

        bool isPreventFadeOut() const;
        void setPreventFadeIn(bool _doPrevent);

        void changeConferenceMode(voip_manager::VideoLayout _layout);

        void callDestroyed();

        void showToast(const QString& _text, int _maxLineCount = 1);

    private:
        void updateVideoDeviceButtonsState();

        enum class MenuAction
        {
            GoToChat,
            ConferenceAllTheSame,
            ConferenceOneIsBig,
            OpenMasks,
            AddUser,
            CallSettings,
            ShareScreen,
            CopyVCSLink,
            InviteVCS
        };

        template<typename Slot>
        void addMenuAction(MenuAction _action, const QString& _addText, Slot&& _slot);
        template<typename Slot>
        void addMenuAction(MenuAction _action, Slot&& _slot);

        static QString menuActionIconPath(MenuAction _action);
        static QString menuActionText(MenuAction _action);
        void resetMenu();
        void showMenuAt(const QPoint& _pos);
        void showMenuAtButton(const VideoPanelParts::PanelButton* _button);
        void closeMenu();

        void switchShareScreen(unsigned int _index);

        void resetPanelButtons(bool _enable = true);

        void showScreenBorder(std::string_view _uid);
        void hideScreenBorder();

        enum class ToastType
        {
            CamNotAllowed,
            DesktopNotAllowed,
            MasksNotAllowed,
            MicNotAllowed,
            LinkCopied,
            EmptyLink
        };

        
        void showToast(ToastType _type);
        void updateToastPosition();
        void hideToast();

    protected:
        void changeEvent(QEvent* _e) override;
        void enterEvent(QEvent* _e) override;
        void leaveEvent(QEvent* _e) override;
        void resizeEvent(QResizeEvent* _e) override;
        void keyReleaseEvent(QKeyEvent* _e) override;
        void paintEvent(QPaintEvent* _e) override;
        void moveEvent(QMoveEvent* _e) override;
        bool event(QEvent* _e) override;

    private:
        QWidget* container_;
        QWidget* parent_;
        QWidget* rootWidget_;

        std::vector<voip_manager::Contact> activeContact_;
        VideoPanelParts::PanelButton* stopCallButton_;
        VideoPanelParts::PanelButton* videoButton_;
        VideoPanelParts::PanelButton* shareScreenButton_;
        VideoPanelParts::PanelButton* microphoneButton_;
        VideoPanelParts::PanelButton* speakerButton_;
        VideoPanelParts::PanelButton* moreButton_;

        QPointer<Previewer::CustomMenu> menu_;
        QPointer<VideoPanelParts::ShareScreenFrame> shareScreenFrame_;
        QPointer<Toast> toast_;

        bool isFadedVisible_;
        bool doPreventFadeIn_;
        bool localVideoEnabled_;
        bool isScreenSharingEnabled_;
        bool isCameraEnabled_;
        bool isConferenceAll_;
        bool isActiveCall_;
        bool isParentMinimizedFromHere_;
        bool isBigConference_;
        bool isMasksAllowed_;

        Qt::WindowStates prevParentState_ = Qt::WindowNoState;
    };
}
