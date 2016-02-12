#ifndef HIDKBMOUSEINPUT_H
#define HIDKBMOUSEINPUT_H


#include "hidinput.h"


class KeyModifierInput : public HidInput {
public:
  KeyModifierInput( const Qt::KeyboardModifiers& keymods, const QVariant& type );

  Qt::KeyboardModifiers   getKeyboardModifiers() const;
  void                    setKeyboardModifiers( const Qt::KeyboardModifiers& keymods );

  bool                    isKeyboardModifiersActive(const Qt::KeyboardModifiers& modifiers ) const;

  QString                 toString() const;
};




class KeyInput : public KeyModifierInput {
public:
  typedef QHash<Qt::Key, bool>   Keymap;


  KeyInput( const Qt::Key& key, const Qt::KeyboardModifiers& keymods, const QVariant& type );
  KeyInput( const Keymap& keymap, const Qt::KeyboardModifiers& keymods, const QVariant& type );

  Keymap                    getKeymap() const;
  void                      setKeymap( const Keymap& keymap );

  bool                      isSingleKey() const;
  void                      setSinglekey( bool state );

  bool                      isKeyActive( Qt::Key key ) const;
  bool                      isKeymapEqual( const Keymap& keymap ) const;

  QString                   toString() const;

private:
  bool                      equals(const HidInput &) const;
};




class KeyPressInput : public KeyInput {
public:
  KeyPressInput( const Keymap& keymap, const Qt::KeyboardModifiers& keymods );
  KeyPressInput( const Qt::Key& key, const Qt::KeyboardModifiers& keymods = Qt::NoModifier );
};

class KeyReleaseInput : public KeyInput {
public:
  KeyReleaseInput( const Keymap& keymap, const Qt::KeyboardModifiers& keymods );
  KeyReleaseInput( const Qt::Key& key, const Qt::KeyboardModifiers& keymods = Qt::NoModifier );

  QString     toString() const;
};















class MouseButtonInput : public KeyModifierInput {
public:
  MouseButtonInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods, const QVariant& type );

  Qt::MouseButtons    getMouseButtons() const;
  void                setMouseButtons( const Qt::MouseButtons& buttons );

  QString     toString() const;

protected:
  bool equals(const HidInput & other) const;
};




class MousePressInput : public MouseButtonInput {
public:
  MousePressInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods = Qt::NoModifier );
};




class MouseReleaseInput : public MouseButtonInput {
public:
  MouseReleaseInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods = Qt::NoModifier );

  QString     toString() const;
};




class MouseDoubleClickInput : public MouseButtonInput {
public:
  MouseDoubleClickInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods = Qt::NoModifier );

  QString     toString() const;
};




class MouseMoveInput : public MouseButtonInput {
public:
  MouseMoveInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods = Qt::NoModifier );

  QString     toString() const;
};







class WheelInput : public KeyModifierInput {
public:
  WheelInput( const Qt::KeyboardModifiers& keymods = Qt::NoModifier );

  QString     toString() const;

private:
  bool      equals(const HidInput &) const;
};












#endif // HIDKBMOUSEINPUT_H
