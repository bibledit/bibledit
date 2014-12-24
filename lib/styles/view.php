<?php
// Userbool1
view.view.userbool1_function = styles_logic.getUserBool1Text (styles_logic.getUserBool1Function (type, subtype));
userbool1 = marker_data["userbool1"];
if (isset (request->query["userbool1"])) {
  userbool1 = Filter_Bool::int (request->query["userbool1"]);
  if (write) database_styles.updateUserbool1 (sheet, style, userbool1);
}
view.view.userbool1_value = styles_logic.OffOnInheritToggleText (userbool1);
view.view.userbool1_toggle = Filter_Bool::not (userbool1);

// Userbool2
view.view.userbool2_function = styles_logic.getUserBool2Text (styles_logic.getUserBool2Function (type, subtype));
userbool2 = marker_data["userbool2"];
if (isset (request->query["userbool2"])) {
  userbool2 = Filter_Bool::int (request->query["userbool2"]);
  if (write) database_styles.updateUserbool2 (sheet, style, userbool2);
}
view.view.userbool2_value = styles_logic.OffOnInheritToggleText (userbool2);
view.view.userbool2_toggle = Filter_Bool::not (userbool2);

// Userbool3
view.view.userbool3_function = styles_logic.getUserBool3Text (styles_logic.getUserBool3Function (type, subtype));
userbool3 = marker_data["userbool3"];
if (isset (request->query["userbool3"])) {
  userbool3 = Filter_Bool::int (request->query["userbool3"]);
  if (write) database_styles.updateUserbool3 (sheet, style, userbool3);
}
view.view.userbool3_value = styles_logic.OffOnInheritToggleText (userbool3);
view.view.userbool3_toggle = Filter_Bool::not (userbool3);

// Userint1.
userint1 = marker_data["userint1"];
switch (styles_logic.getUserInt1Function (type, subtype)) {
  case UserInt1None :
    break;
  case UserInt1NoteNumbering :
    view.view.userint1_notenumbering = true;
    if (isset (request->query["notenumbering"])) {
      dialog_list = new Dialog_List (array ("sheet", "style"), gettext("Would you like to change the numbering of the note?"), "", "");
      styles_logic = Styles_Logic::getInstance();
      for (i = NoteNumbering123; i <= NoteNumberingUser; i++) {
        dialog_list.add_row (styles_logic.noteNumberingText (i), "&userint1=i");
      }
      dialog_list.run ();
      die;
    }
    if (isset (request->query["userint1"])) {
      userint1 = request->query["userint1"];
      if (write) database_styles.updateUserint1 (sheet, style, userint1);
    }
    view.view.userint1 = styles_logic.noteNumberingText (userint1);
    break;
  case UserInt1TableColumnNumber :
    view.view.userint1_columnnumber = true;
    if (isset (request->query["userint1"])) {
      dialog_entry = new Dialog_Entry (standard_page_query, gettext("Please enter a column number between 1 and 4"), userint1, "userint1", gettext ("This is the column number for the style. The first columm is number 1."));
      die;
    }
    if (isset(_POST["userint1"])) {
      value = _POST["entry"];
      value = Filter_Numeric::integer_in_string (value);
      if ((value >= 1) && (value <= 4)) {
        userint1 = value;
        if (write) database_styles.updateUserint1 (sheet, style, userint1);
      }
    }
    view.view.userint1 = userint1;
    break;
}

// Userint2.
userint2 = marker_data["userint2"];
switch (styles_logic.getUserInt2Function (type, subtype)) {
  case UserInt2None :
    break;
  case UserInt2NoteNumberingRestart :
    view.view.userint2_notenumberingrestart = true;
    if (isset (request->query["notenumberingrestart"])) {
      dialog_list = new Dialog_List (array ("sheet", "style"), gettext("Would you like to change when the note numbering restarts?"), "", "");
      styles_logic = Styles_Logic::getInstance();
      for (i = NoteRestartNumberingNever; i <= NoteRestartNumberingEveryChapter; i++) {
        dialog_list.add_row (styles_logic.noteRestartNumberingText (i), "&userint2=i");
      }
      dialog_list.run ();
      die;
    }
    if (isset (request->query["userint2"])) {
      userint2 = request->query["userint2"];
      if (write) database_styles.updateUserint2 (sheet, style, userint2);
    }
    view.view.userint2 = styles_logic.noteRestartNumberingText (userint2);
    break;
  case UserInt2EndnotePosition :
    view.view.userint2_endnoteposition = true;
    if (isset (request->query["endnoteposition"])) {
      dialog_list = new Dialog_List (array ("sheet", "style"), gettext("Would you like to change the position where to dump the endnotes?"), "", "");
      styles_logic = Styles_Logic::getInstance();
      for (i = EndNotePositionAfterBook; i <= EndNotePositionAtMarker; i++) {
        dialog_list.add_row (styles_logic.endNotePositionText (i), "&userint2=i");
      }
      dialog_list.run ();
      die;
    }
    if (isset (request->query["userint2"])) {
      userint2 = request->query["userint2"];
      if (write) database_styles.updateUserint2 (sheet, style, userint2);
    }
    view.view.userint2 = styles_logic.endNotePositionText (userint2);
    break;
}

// Userint3 not yet used.

// Userstring1.
userstring1 = marker_data["userstring1"];
switch (styles_logic.getUserString1Function (type, subtype)) {
  case UserString1None :
    break;
  case UserString1NoteNumberingSequence :
    if (userint1 == NoteNumberingUser) {
      view.view.userstring1_numberingsequence = true;
      userstring_question = gettext("Please enter a new note numbering sequence");
      userstring1_help = gettext("This gives a sequence for numbering the notes. When for example § † * is entered, the numbering goes like §, †, *, §, †, *, and so forth. Any sequence of characters can be used. Spaces should separate the characters");
    }
    break;
  case UserString1WordListEntryAddition :
    view.view.userstring1_wordlistaddition = true;
    userstring1_question = gettext("Please enter a new addition to the word list entry");
    userstring1_help = gettext("This given an optional string to be added after each definition in the body of text. In some Bibles the unusual words are marked with an asterisk and then explained in a glossary. If you would enter the asterisk here, or indeed any string, Bibledit-Web would include this in the exported documents.");
    break;
}
if (isset (request->query["userstring1"])) {
  dialog_entry = new Dialog_Entry (standard_page_query, userstring1_question, userstring1, "userstring1", userstring1_help);
  die;
}
if (isset(_POST["userstring1"])) {
  userstring1 = _POST["entry"];
  if (write) database_styles.updateUserstring1 (sheet, style, userstring1);
}
if (userstring1 == "") userstring1 = "--";
view.view.userstring1 = filter_string_sanitize_html (userstring1);

// Userstring2
userstring2 = marker_data["userstring2"];
switch (styles_logic.getUserString2Function (type, subtype)) {
  case UserString2None :
    break;
  case UserString2DumpEndnotesHere :
    if (userint2 == EndNotePositionAtMarker) {
      view.view.userstring2_dumpendnotes = true;
      userstring2_question = gettext("Please enter a marker at which the endnotes should be dumped");
      userstring2_info = gettext("The marker is to be given without the backslash, e.g. \"zendnotes\".");
    }
    break;
}
if (isset (request->query["userstring2"])) {
  dialog_entry = new Dialog_Entry (standard_page_query, userstring2_question, userstring2, "userstring2", userstring2_info);
  die;
}
if (isset(_POST["userstring2"])) {
  userstring2 = _POST["entry"];
  if (write) database_styles.updateUserstring2 (sheet, style, userstring2);
}
if (userstring2 == "") userstring2 = "--";
view.view.userstring2 = filter_string_sanitize_html (userstring2);

// Recreate stylesheets after editing a style.
if ((count (request->query) != 2) || (count (_POST) != 0)) {
  Styles_Sheets::create_all ();
}

// Userstring3 not yet used.

view.render ("view.php");

Assets_Page::footer ();

?>
