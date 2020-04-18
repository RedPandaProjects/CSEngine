#include "CSEngine.hpp"
#include "engine/keydefs.h"
CSKeyState* GKeyState = 0;

CSKeyState::CSKeyState()
{
    FunctionToC();
}

CSKeyState::~CSKeyState()
{
}

void CSKeyState::ClearState()
{
    for (bsize i = 0; i < 256; i++)
    {
        m_Keys[i].Down = false;
    }
}

void CSKeyState::SetBinding(int key, BearStringConteniarAnsi Binding)
{
    if (key >= 0 && key <= 255)
        m_Keys[key].Binding = Binding;
}

BearStringConteniarAnsi CSKeyState::GetBinding(int key)
{
    if (key >= 0 && key <= 255)
        return m_Keys[key].Binding;
    return BearStringConteniarAnsi();
}

void CSKeyState::Event(BearInput::Key Key, bool Down)
{
    int basic_key = -1;
    if (Key >= BearInput::KeyA && Key <= BearInput::KeyZ)
    {
        basic_key = Key - BearInput::KeyA + 'a';
    }
    else if (Key >= BearInput::KeyNum0 && Key <= BearInput::KeyNum9)
    {
        basic_key = Key - BearInput::KeyNum0 + '0';
    }
    else if (Key >= BearInput::KeyF1 && Key <= BearInput::KeyF12)
    {
        basic_key = Key - BearInput::KeyF1 + K_F1;
    }
    else
    {
        switch (Key)
        {
        case BearInput::KeyReturn:
            basic_key = K_ENTER;
            break;
        case BearInput::KeyEscape:
            basic_key = K_ESCAPE;
            break;
        case BearInput::KeyRControl:
        case BearInput::KeyLControl:
            basic_key = K_CTRL;
            break;
        case BearInput::KeyRShift:
        case BearInput::KeyLShift:
            basic_key = K_SHIFT;
            break;
        case BearInput::KeyRAlt:
        case BearInput::KeyLAlt:
            basic_key = K_ALT;
            break;
        case BearInput::KeySpace:
            basic_key = K_SPACE;
            break;
        case BearInput::KeyBackSpace:
            basic_key = K_BACKSPACE;
            break;
        case BearInput::KeyTab:
            basic_key = K_TAB;
            break;
        case BearInput::KeyPause:
            basic_key = K_PAUSE;
            break;
        case BearInput::KeyLeft:
            basic_key = K_LEFTARROW;
            break;
        case BearInput::KeyRight:
            basic_key = K_RIGHTARROW;
            break;
        case BearInput::KeyUp:
            basic_key = K_UPARROW;
            break;
        case BearInput::KeyDown:
            basic_key = K_DOWNARROW;
            break;
        case BearInput::KeyEnd:
            basic_key = K_END;
            break;
        case BearInput::KeyHome:
            basic_key = K_HOME;
            break;
        case BearInput::KeyInsert:
            basic_key = K_INS;
            break;
        case BearInput::KeyDelete:
            basic_key = K_DEL;
            break;
        case BearInput::KeyAdd:
            basic_key = VK_ADD;
            break;
        case BearInput::KeySubtract:
            basic_key = '-';
            break;
        case BearInput::KeyMultiply:
            basic_key = '*';
            break;
        case BearInput::KeyDivide:
            basic_key = K_KP_SLASH;
            break;
        case BearInput::KeyComma:
            basic_key = ',';
            break;
        case BearInput::KeySlash:
            basic_key = '/';
            break;
        case BearInput::KeyBackSlash:
            basic_key = '\\';
            break;
        case BearInput::KeyEqual:
            basic_key = '=';
            break;
        case BearInput::KeyPeriod:
            basic_key = '.';
            break;
        case BearInput::KeySemiColon:
            basic_key = ';';
            break;
        case BearInput::KeyPageUp:
            basic_key = K_PGUP;
            break;
        case BearInput::KeyLBracket:
            basic_key = '[';
            break;
        case BearInput::KeyRBracket:
            basic_key = ']';
            break;
        case BearInput::KeyTilde:
            basic_key = '`';
            break;
        case BearInput::KeyPageDown:
            basic_key = K_PGDN;
            break;

        case BearInput::KeyMouseLeft:
            basic_key = K_MOUSE1;
            break;
        case BearInput::KeyMouseRight:
            basic_key = K_MOUSE3;
            break;
        case BearInput::KeyMouseMiddle:
            basic_key = K_MOUSE2;
            break;
        default:
            break;
        }
    }
    if (basic_key == -1)return;
    GMenuManager->KeyEvent(basic_key, Down);
    m_Keys[basic_key].Down = Down;
}

bool CSKeyState::IsDown(int key)
{
    if (key >= 0 && key <= 255)
        return m_Keys[key].Down ;
    return false;
}

