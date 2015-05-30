LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libsqlite
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libsqlite.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../sqlite/jni
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libxml2
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libxml2.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../xml2/jni
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libbibledit
# LOCAL_SRC_FILES := bibledit.cpp library/bibledit.cpp library/locks.c webserver/webserver.cpp webserver/http.cpp webserver/request.cpp webserver/io.c bootstrap/bootstrap.cpp filter/url.cpp filter/string.cpp filter/roles.cpp filter/md5.cpp filter/usfm.cpp filter/archive.cpp filter/text.cpp filter/passage.cpp filter/customcss.cpp filter/bibleworks.cpp filter/git.cpp filter/diff.cpp filter/abbreviations.cpp filter/shell.cpp filter/merge.cpp filter/date.cpp flate/flate.cpp assets/view.cpp assets/page.cpp assets/header.cpp index/index.cpp index/listing.cpp config/globals.cpp menu/logic.cpp menu/main.cpp menu/user.cpp locale/translate.cpp locale/logic.cpp database/config/general.cpp database/config/bible.cpp database/config/user.cpp database/users.cpp database/logs.cpp database/sqlite.cpp database/styles.cpp database/bibles.cpp database/search.cpp database/books.cpp database/bibleactions.cpp database/check.cpp database/commits.cpp database/confirm.cpp database/ipc.cpp database/jobs.cpp database/kjv.cpp database/morphhb.cpp database/sblgnt.cpp database/offlineresources.cpp database/sprint.cpp database/mail.cpp database/navigation.cpp database/usfmresources.cpp database/mappings.cpp database/noteactions.cpp database/versifications.cpp database/modifications.cpp database/notes.cpp database/volatile.cpp database/maintenance.cpp session/logic.cpp session/login.cpp session/logout.cpp session/password.cpp session/signup.cpp parsewebdata/ParseMultipartFormData.cpp parsewebdata/ParseWebData.cpp setup/index.cpp setup/logic.cpp journal/index.cpp journal/logic.cpp styles/logic.cpp styles/indext.cpp styles/indexm.cpp styles/sheetm.cpp styles/view.cpp styles/css.cpp styles/sheets.cpp text/text.cpp esword/text.cpp onlinebible/text.cpp html/text.cpp html/header.cpp odf/text.cpp timer/index.cpp tasks/logic.cpp tasks/run.cpp config/logic.cpp bible/logic.cpp bible/manage.cpp bible/settings.cpp bible/book.cpp bible/chapter.cpp bible/import_usfm.cpp bible/import_bibleworks.cpp bible/import_task.cpp bible/abbreviations.cpp bible/order.cpp bible/css.cpp bible/editing.cpp notes/logic.cpp notes/actions.cpp notes/clean.cpp notes/note.cpp notes/status-1.cpp notes/assign-1.cpp notes/cleancli.cpp notes/notes.cpp notes/status-n.cpp notes/assign-n.cpp notes/click.cpp notes/poll.cpp notes/summary.cpp notes/bible-1.cpp notes/comment.cpp notes/select.cpp notes/unassign-n.cpp notes/bible-n.cpp notes/create.cpp notes/severity-1.cpp notes/verses.cpp notes/bulk.cpp notes/editsource.cpp notes/index.cpp notes/severity-n.cpp trash/handler.cpp sync/logic.cpp help/index.cpp confirm/worker.cpp email/index.cpp email/send.cpp email/receive.cpp user/notifications.cpp user/account.cpp manage/indexing.cpp manage/users.cpp manage/exports.cpp manage/hyphenation.cpp manage/hyphenate.cpp administration/language.cpp administration/timezone.cpp collaboration/index.cpp collaboration/flash.cpp collaboration/open.cpp collaboration/password.cpp collaboration/secure.cpp collaboration/link.cpp collaboration/direction.cpp search/rebibles.cpp search/renotes.cpp access/user.cpp access/bible.cpp dialog/entry.cpp dialog/list.cpp dialog/yes.cpp dialog/color.cpp dialog/books.cpp dialog/upload.cpp fonts/logic.cpp fonts/index.cpp versification/index.cpp versification/system.cpp versification/logic.cpp book/create.cpp compare/index.cpp compare/compare.cpp jobs/index.cpp editverse/index.cpp editverse/load.cpp editverse/save.cpp editone/index.cpp editone/load.cpp editone/save.cpp navigation/passage.cpp navigation/update.cpp navigation/poll.cpp ipc/focus.cpp ipc/notes.cpp checksum/logic.cpp editusfm/focus.cpp editusfm/index.cpp editusfm/load.cpp editusfm/offset.cpp editusfm/save.cpp editor/styles.cpp editor/export.cpp editor/import.cpp edit/edit.cpp edit/focus.cpp edit/id.cpp edit/index.cpp edit/load.cpp edit/offset.cpp edit/save.cpp edit/styles.cpp edit/logic.cpp search/search.cpp search/index.cpp search/select.cpp search/replace.cpp search/getids.cpp search/replacepre.cpp search/replacego.cpp search/search2.cpp search/replace2.cpp search/replacepre2.cpp search/getids2.cpp search/replacego2.cpp search/similar.cpp search/strongs.cpp search/strong.cpp search/originals.cpp tmp/tmp.cpp workbench/index.cpp workbench/logic.cpp workbench/settings.cpp workbench/organize.cpp sendreceive/logic.cpp sendreceive/index.cpp sendreceive/sendreceive.cpp sendreceive/settings.cpp sendreceive/bibles.cpp sendreceive/usfmresources.cpp sendreceive/externalresources.cpp sendreceive/notes.cpp demo/logic.cpp client/index.cpp client/logic.cpp sync/setup.cpp sync/settings.cpp sync/bibles.cpp sync/usfmresources.cpp sync/externalresources.cpp sync/notes.cpp resource/index.cpp resource/organize.cpp resource/logic.cpp resource/get.cpp resource/external.cpp resource/bible2resource.cpp resource/convert2resource.cpp resource/convert2bible.cpp resource/manage.cpp resource/admin.cpp resource/print.cpp resource/download.cpp json/json.c mapping/index.cpp mapping/map.cpp statistics/statistics.cpp changes/change.cpp changes/changes.cpp changes/logic.cpp changes/manage.cpp changes/modifications.cpp sprint/burndown.cpp sprint/index.cpp checks/run.cpp checks/headers.cpp checks/index.cpp checks/logic.cpp checks/sentences.cpp checks/settings.cpp checks/settingspatterns.cpp checks/settingssentences.cpp checks/space.cpp checks/suppress.cpp checks/usfm.cpp checks/verses.cpp checks/versification.cpp consistency/index.cpp consistency/input.cpp consistency/logic.cpp consistency/poll.cpp export/esword.cpp export/html.cpp export/index.cpp export/info.cpp export/logic.cpp export/odt.cpp export/onlinebible.cpp export/textusfm.cpp export/usfm.cpp export/web.cpp xrefs/clear.cpp xrefs/extract.cpp xrefs/help.cpp xrefs/index.cpp xrefs/insert.cpp xrefs/interpret.cpp xrefs/move.cpp xrefs/next.cpp xrefs/source.cpp xrefs/target.cpp xrefs/translate.cpp webbible/search.cpp debug/index.cpp
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libbibledit.a
# LOCAL_C_INCLUDES += $(LOCAL_PATH)/../sqlite/jni
# LOCAL_C_INCLUDES += $(LOCAL_PATH)/../xml2/jni/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/library
LOCAL_CPP_FEATURES := rtti exceptions
# include $(BUILD_STATIC_LIBRARY)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libjavawrapper
LOCAL_SRC_FILES := javawrapper.c
LOCAL_LDLIBS    := -lgnustl_shared
LOCAL_STATIC_LIBRARIES := bibledit sqlite xml2
LOCAL_CPP_FEATURES := rtti exceptions
include $(BUILD_SHARED_LIBRARY)



# LOCAL_CPP_FEATURES += exceptions
