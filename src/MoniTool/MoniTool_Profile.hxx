// Created on: 1998-12-14
// Created by: Christian CAILLET
// Copyright (c) 1998-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _MoniTool_Profile_HeaderFile
#define _MoniTool_Profile_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <TCollection_AsciiString.hxx>
#include <MMgt_TShared.hxx>
#include <Standard_CString.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <Standard_Boolean.hxx>
class Dico_DictionaryOfTransient;
class MoniTool_Option;
class TCollection_AsciiString;
class Standard_Transient;


class MoniTool_Profile;
DEFINE_STANDARD_HANDLE(MoniTool_Profile, MMgt_TShared)

//! A Profile gives access to a set of options :
//! - it defines and gives access to options, by names specific
//! to this Profile, defaulted to basic names of the Options
//! - it records one or more Configurations, each of one is
//! identified by a name, and commands for each option a switch
//! value
//! - a Configuration may be partial, i.e. concern only a subset
//! of the complete list of options
//! - Values are accessed through a Current Configuration, which
//! name is fixed as ".", which can cumulate several
//! configurations (especially when they are partial)
//!
//! It may also bring a "fast" access to values, which has been
//! formerly computed from current configuration or another given
//! one. But its up-to-date status regarding existing
//! configurations is not checked : it may be recomputed or
//! cleared at any time. If not set, the current configuration
//! is then taken
class MoniTool_Profile : public MMgt_TShared
{

public:

  
  //! Creates an empty Profile, with an empty Current Configuration
  //! (named ".")
  Standard_EXPORT MoniTool_Profile();
  
  //! Adds an Option, with a Name, by default takes the Name of the
  //! Option
  Standard_EXPORT void AddOption (const Handle(MoniTool_Option)& option, const Standard_CString name = "");
  
  //! Returns an Option from its Name. Null Handle if unknown
  Standard_EXPORT Handle(MoniTool_Option) Option (const Standard_CString name) const;
  
  //! Returns the list of Option Names
  Standard_EXPORT Handle(TColStd_HSequenceOfAsciiString) OptionList() const;
  
  //! Returns the list of Option Names which are defined on a
  //! TypedValue/Static, i.e. for Basic Type
  Standard_EXPORT Handle(TColStd_HSequenceOfAsciiString) TypedValueList() const;
  
  //! Adds a new Configuration, yet empty, then filled by AddSwitch
  //! If <confname> already existed, it is replaced from scratch
  //! While AddConf creates new Conf only if it does not yet exist
  //!
  //! Remark : the Current Configuration "." may not be changed
  Standard_EXPORT void NewConf (const Standard_CString confname);
  
  //! Adds a new Configuration, yet empty, then filled by AddSwitch
  //! If <confname> already existed, it is kept as it is
  //! While NewConf restrts from scratch anyway
  //!
  //! Remark : the Current Configuration "." may not be changed
  Standard_EXPORT void AddConf (const Standard_CString confname);
  
  //! Tells if <confname> is known as a Configuration
  Standard_EXPORT Standard_Boolean HasConf (const Standard_CString confname) const;
  
  //! Returns the list of Configuration Names, including Current "."
  Standard_EXPORT Handle(TColStd_HSequenceOfAsciiString) ConfList() const;
  
  //! Clears a configuration (does not remove it)
  //! A cleared configuration has no effect, it can be reloaded
  Standard_EXPORT Standard_Boolean ClearConf (const Standard_CString confname);
  
  //! Adds the definitions from the configuration <otherconf> to
  //! those of <confname>
  //! Returns True if done, False if confname or otherconf are unknown
  Standard_EXPORT Standard_Boolean AddFromOtherConf (const Standard_CString confname, const Standard_CString otherconf);
  
  //! Sets the definitions of a configuration from the actual state
  //! (current configuration plus current state of options for those
  //! which are not recorded in current conf)
  Standard_EXPORT Standard_Boolean SetFromCurrent (const Standard_CString confname);
  
  //! Adds to the Configuration <confname>, for the option <optname>
  //! the command to switch it to <casename>
  //! If <casename> is not given, records the actual current
  //! CaseName of this Option
  //! Returns True if done, False if <confname> or <optname> is
  //! unknown, or <casename> not allowed for <optname>
  Standard_EXPORT Standard_Boolean AddSwitch (const Standard_CString confname, const Standard_CString optname, const Standard_CString casename = "");
  
  //! Removes from the Configuration <confname>, the switch for the
  //! option <optname>. Hence, it will consider the basic default
  //! switch for this option
  //! Returns True if done or switch was not recorded, False if
  //! <confname> or <optname> is unknown
  Standard_EXPORT Standard_Boolean RemoveSwitch (const Standard_CString confname, const Standard_CString optname);
  
  //! Returns the list of switches properly concerned by a config :
  //! two lists in parallel, <optlist> lists the names of options
  //! and for each one, <caselist> gives the case for this config
  Standard_EXPORT void SwitchList (const Standard_CString confname, Handle(TColStd_HSequenceOfAsciiString)& optlist, Handle(TColStd_HSequenceOfAsciiString)& caselist) const;
  
  //! Copies (Cumulates) the definition of <confname> to current
  //! Records the current name as the last current setting
  //!
  //! Returns True if done, False (and does nothing> if <confname>
  //! is not recorded as a Configuration
  //! If no SetCurrent has been called, the last added one is taken
  //!
  //! Remark : SetCurrent is for the Profile : the individual
  //! options are not switched themselves, they are only queried
  //! To switch the options themselves, see below RecordCurrent
  //!
  //! Remark : for Options which are listed in the Profile but not
  //! in the current Configuration, their current switch is taken
  Standard_EXPORT Standard_Boolean SetCurrent (const Standard_CString confname);
  
  //! Records the switches of current configuration in the options
  //! themselves
  Standard_EXPORT void RecordCurrent();
  
  //! Returns the name of last current configuration
  Standard_EXPORT const TCollection_AsciiString& Current() const;
  
  //! Returns the selected case name of option <optname> in current
  //! configuration
  //! Empty String if <optname> unknown
  //!
  //! If <proper> is False (D), if <optname> is not recorded in the
  //! current configuration, the own current name of the option
  //! itself is taken
  //! Else, an empty string is returned
  Standard_EXPORT TCollection_AsciiString CaseName (const Standard_CString optname, const Standard_Boolean proper = Standard_False) const;
  
  //! Returns the Value of the switch selected for option <optname>
  //! in current configuration
  //! Null Handle if <optname> unknown
  //! To be down-casted as needed before use
  Standard_EXPORT Handle(Standard_Transient) CaseValue (const Standard_CString optname) const;
  
  //! Returns the Value of the switch selected for option <optname>
  //! in current configuration
  //! Returns True if done, False + Null Handle if <optname> unknown
  //! This way of returning a Transient, bound with the Type Control
  //! avoids DownCast and ensures the value is directly usable
  Standard_EXPORT Standard_Boolean Value (const Standard_CString optname, Handle(Standard_Transient)& val) const;
  
  //! For the Options which are defined with a TypedValue/Static,
  //! takes the value defined in current configuration, and sets
  //! the TypedValue with it
  //!
  //! If <proper> is False (D), takes the current value, whatever
  //! it is properly recorded in current configuration or not
  //! If <proper> is True, sets the value only if it is properly
  //! recorded in the current configuration
  //!
  //! By default, considers all the Options with a TypedValue
  //! If <name> is given, considers the Options of which the name
  //! begins by <name>
  Standard_EXPORT void SetTypedValues (const Standard_Boolean proper = Standard_False, const Standard_CString name = "") const;
  
  //! Recomputes fast access from the definition of a configuration
  //! By default, the current one
  //! Else, a configuration identified by its name
  Standard_EXPORT void SetFast (const Standard_CString confname = "");
  
  //! Clears definitions of fast access
  Standard_EXPORT void ClearFast();
  
  //! Gets the value corresponding to Option <optname>, as fast
  //! If not defined as fast, check "normal" access by calling Value
  //! Returns True if OK (either Fast or normal), False if not found
  //! (if not found, <val> is returned Null Handle)
  Standard_EXPORT Standard_Boolean FastValue (const Standard_CString optname, Handle(Standard_Transient)& val) const;




  DEFINE_STANDARD_RTTIEXT(MoniTool_Profile,MMgt_TShared)

protected:




private:

  
  //! Internal
  Standard_EXPORT Handle(Dico_DictionaryOfTransient) Conf (const Standard_CString confname) const;

  Handle(Dico_DictionaryOfTransient) theopts;
  Handle(Dico_DictionaryOfTransient) theconfs;
  TCollection_AsciiString thecurname;
  Handle(Dico_DictionaryOfTransient) thecurconf;
  Handle(Dico_DictionaryOfTransient) thefastval;


};







#endif // _MoniTool_Profile_HeaderFile
