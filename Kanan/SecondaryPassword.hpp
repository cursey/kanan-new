#pragma once

#include <cstdint>

#include "Mod.hpp"
#include "Mabinogi.hpp"
#include "FunctionHook.hpp"
#include <memory>

namespace kanan {
#pragma pack(push, 1)
    // esl::_rect<short>
    struct _rect_short
    {
        uint16_t X;
        uint16_t Y;
        uint16_t Width;
        uint16_t Height;
    };

    // number and type of arguments can be incorrect, please check before use
    class CWindow
    {
    public:
        virtual void Destroy1(char a2) {}
        virtual void Destroy() {}
        virtual void Close() {}
        virtual bool IsDestroying() { return true; }
        virtual int GetClassNameW(int a1) { return 0; }
        virtual void Go(unsigned int a2) {}
        virtual void Render() {}
        virtual void RenderDebug() {}
        virtual void DeclareChild() {}
        virtual void UndeclareChild() {}
        virtual void ProcessStaticChild() {}
        virtual void SetZDepth(float a2) {}
        virtual void SetPosition(__int16 a2, int a3) {}
        virtual void SetExtent(__int16 a2, int a3) {}
        virtual void SetAdjustExtent(int a2, int a3) {}
        virtual void SetTitle(CString **a2) {}
        virtual void SetColor(unsigned int a2, unsigned int a3) {}
        virtual void SetScrollBarColor(unsigned int a2, unsigned int a3) {}
        virtual void SetBaseColor() {}
        virtual void SetTextColor() {}
        virtual void nullsub1() {} //new
        virtual void FadeIn(unsigned int a2) {}
        virtual void FadeIn2(unsigned int a2, int a3) {} //new
        virtual void EndAnimation() {}
        virtual void EndChildAnimation(int a2) {}
        virtual void SetEnable(bool a2) {}
        virtual void ClientToScreen(__int16 *a2, __int16 *a3) {}
        virtual void ScreenToClient(__int16 *a2, __int16 *a3) {}
        virtual int GetGradationColor(__int16 a2) { return 0; }
        virtual int GetScrollBarColor(int a2) { return 0; }
        virtual void GetClientExtent(unsigned __int16 *a2, unsigned __int16 *a3) {}
        virtual void GetCursorPos(__int16 *a2, __int16 *a3) {}
        virtual int GetGoodCursor(int a1, int a2) { return 0; }
        virtual unsigned int GetInnerWidth() { return 0; }
        virtual unsigned int GetInnerHeight() { return 0; }
        virtual int GetXOffset() { return 0; }
        virtual int GetYOffset() { return 0; }
        virtual void SetXOffset(unsigned __int16 a2) {}
        virtual void SetYOffset(unsigned __int16 a2) {}
        virtual double GetXOffsetRatio() { return 0.0; }
        virtual double GetYOffsetRatio() { return 0.0; }
        virtual void SetXOffsetByRatio(float a2) {}
        virtual void SetYOffsetByRatio(float a2) {}
        virtual unsigned __int16 GetXOffsetGrid(bool a2) { return 0; }
        virtual unsigned __int16 GetYOffsetGrid(bool a2) { return 0; }
        virtual void Declare() {}
        virtual void Undeclare() {}
        virtual void ProcessStatic() {}
        virtual void UpdateWindow() {}
        virtual char OnReceiveMessage(int a1, int a2, int a3) { return 0; }
        virtual void ProcessCapture(int a1, int a2, int a3) {}
        virtual void OnMouseLButtonDown(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseLButtonUp(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseLButtonRepeat(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseLButtonDblClk(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseRButtonDown(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseRButtonUp(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseRButtonRepeat(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseRButtonDblClk(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseMButtonDown(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseMButtonUp(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseMButtonRepeat(__int16 a2, __int16 a3, bool a4) {}
        virtual void OnMouseMButtonDblClk(__int16 a2, __int16 a3, bool a4) {}
        virtual bool OnMouseWheel(__int16 a2, __int16 a3, __int16 a4, bool a5) { return true; }
        virtual void OnMouseMove(__int16 a2, __int16 a3) {}
        virtual void OnEnter() {}
        virtual void OnLeave() {}
        virtual void OnVisibleChanged(bool a2) {}
        virtual void OnIMEControlKey(int a1, int a2) {}
        virtual bool OnIMEKeyDown(unsigned int a2) { return true; }
        virtual void OnIMEKeyUp(unsigned int a2) {}
        virtual void OnIMEChar(wchar_t a2) {}
        virtual void OnIMEHotKey(wchar_t a2) {}
        virtual void OnIMEComposite(const int *a2, unsigned int a3, unsigned int a4) {}
        virtual void OnIMEComplete(const int *a2) {}
        virtual void OnIMESetConversionMode(bool a2) {}
        virtual void OnFocus(int a2, unsigned int a3) {}
        virtual void OnChildFocus(unsigned int a2, bool a3, unsigned int a4) {}
        virtual int OnEndAnimation() { return 0; }
        virtual void OnResized(int a4, int a5) {}
        virtual void OnEnabled(bool a2) {}
        virtual bool CanGetIMEFocus() { return true; }
        virtual void GetExtent(unsigned __int16 *a2, unsigned __int16 *a3) {}
        virtual void *Unknown_v293(int *a1) { return nullptr; }
        virtual CString *GetTitle(CString *a2) { return nullptr; }
        virtual char ApplyAttributes(const int *a3, int *a4) { return 0; }
        virtual bool CreateFromLayout(const int *a3, int *a4) { return true; }
        virtual char OnPreCreate(int *a2) { return 0; }
        virtual void OnPostCreate(int *a2) {}
        virtual void *ConvertVariable(void *a1, int a2, int a3) { return nullptr; }

    public:
        uint32_t windowType;
        uint32_t Id;
        CWindow *Parent;
        uint32_t field_10;
        uint32_t OP;
        uint32_t ST;
        CString *Title;
        uint8_t ToolTip[4];
        uint32_t *CompiledTitle;
        uint8_t InterfaceSound[20];
        _rect_short Position;
        _rect_short PickArea;
        float ZDepth;
        uint32_t Color1;
        uint32_t Color2;
        uint32_t ScrollBarColor1;
        uint32_t ScrollBarColor2;
        uint32_t TextPalette[16];
        uint8_t isInWindowMgr;
        uint8_t field_A1;
        uint8_t field_A2;
        uint8_t field_A3;
        uint8_t IsAnimating;
        uint8_t gap_A5[3];
        uint32_t *TimeVar;
        uint8_t gap_AC[4];
        uint32_t new1;
        CString *FullName;
        uint8_t field_B4;
        uint8_t isVisible;
        uint8_t isEnabled;
        uint8_t gap_B7;
        uint8_t ResizeDependency[24];
        uint32_t Attributes;
        uint32_t *LayoutNode;
        uint8_t gap_D4[4];
        CString *LayoutName;
        CString *BaseClass;
    };

    class CButton : public CWindow
    {
    public:
        uint16_t field_E0;
        uint16_t ButtonImageType;
        uint16_t field_E4;
        uint16_t field_E6;
        uint16_t field_E8;
        uint8_t gap_EA[2];
        uint32_t field_EC;
        uint32_t field_F0;
        uint32_t field_F4;
        uint8_t gap_F8[4];
        uint32_t field_FC;
        uint8_t str1[4];
        uint32_t DefaultTextColor;
        uint16_t field_108;
        uint8_t gap_10A[2];
        uint16_t field_10C;
        uint8_t gap_10E[2];
        uint8_t AutoTick;
        uint8_t gap_111[3];
        uint32_t field_114;
        uint32_t IsFlashing;
        uint8_t gap_11C[8];
        uint32_t field_124;
        uint32_t field_128;
        uint32_t field_12C;
    };

    class CEdit : public CWindow
    {
    public:
        uint8_t gap_DD[4];
        uint32_t MaxLength;
        uint8_t str1[4];
        uint8_t CandidateString[4];
        uint32_t field_F0;
        uint32_t field_F4;
        uint8_t Declare[4];
        uint8_t HelpMessage[4];
        uint32_t Colors[16];
        uint8_t isHelpMessage;
        uint8_t gap_141[3];
        uint32_t BGColor;
        uint16_t InnerYMargin;
    };

    class CView : public CWindow
    {
    public:
        uint32_t field_E0;
        uint32_t field_E4;
        uint32_t field_E8;
        CButton *CloseButton;
        CButton *MinimizeButton;
        uint8_t field_F4;
        uint8_t gap_F5;
        uint16_t field_F6;
        uint16_t field_F8;
        uint16_t WindowType;
        uint16_t WindowImageType;
        uint16_t field_FE;
        uint16_t Icon;
        uint16_t CapturePosX;
        uint16_t CapturePosY;
        uint16_t CapturePivotX;
        uint16_t CapturePivotY;
        uint16_t CaptureX;
        uint16_t CaptureY;
        uint8_t gap2121212[2];
        uint32_t Grid;
        uint8_t field_114;
        uint8_t gap_115[5];
        uint16_t MinimumExtentX;
        uint16_t MinimumExtentY;
        uint16_t MaximumExtentX;
        uint16_t MaximumExtentY;
        uint8_t gap_122[2];
        CWindow *ChildModalWindow;
        uint32_t field_128;
        uint32_t field_12C;
        uint32_t field_130;
        uint32_t field_134;
        uint8_t gap_138[4];
        uint32_t SomeViewType;
        uint8_t gap_140[4];
        uint8_t field_144;
        uint8_t field_145;
        uint8_t field_146;
        uint8_t field_147;
        uint8_t field_148;
        uint8_t field_149;
    };


    class CVisualKeyboardView : public CView
    {
    public:
        uint8_t gap_14A[2];
        uint32_t field_14C;
        uint32_t *letterVector;
        uint32_t *field_154;
        CEdit *inputBox;
        CButton *shuffleButton;
        CButton *backSpaceButton;
        CButton *defaultButton;
        CButton *okButton;
        CButton *cancelButton;
        CButton *LShiftButton;
        CButton *RShiftButton;
        CButton *toggleButton;
        uint8_t isPassword;
        uint8_t field_17D;
        uint8_t isUpperCase;
        uint8_t gap_17F;
        uint32_t field_180;
        uint8_t field_184;
        uint8_t gap_185[3];
        uint32_t Mode;
        uint8_t isPostCreateDone;
        uint8_t gap_18D[3];
        uint32_t *lowercaseLetters;
        uint32_t *uppercaseLetters;
        uint32_t *hangulLetters1;
        uint32_t *hangulLetters2;
        uint8_t isFirstUse;
    };

#pragma pack(pop)

    class SecondaryPassword : public Mod {
    public:
        SecondaryPassword();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        CString *password;
        char savedPassword[60];
        std::unique_ptr<FunctionHook> m_setCVisualKeyboardViewOnPostCreateHook;
        
        static void __fastcall hookedCVisualKeyboardViewOnPostCreate(CVisualKeyboardView*, uint32_t, int);
        void setup();

        // Utility
        std::string wstringToHex(std::wstring);
        std::string memoryToHex(void*, int);
        
        //CString stuff
        CString *(__cdecl *m_CStringAllocate)(int);
        bool InitCStrings();
        CString* CStringConstructor(wchar_t*);
    };
}
