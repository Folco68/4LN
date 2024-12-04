    /***************************************************************************
     *                                                                         *
     *                                   4LN                                   *
     *                                                                         *
     *                      Multiple destinations copier                       *
     *                                                                         *
     **************************************************************************/


About
-----

Author:                           Martial Demolins
Mail:                             PLACEHOLDER_EMAIL_PERSONAL, PLACEHOLDER_EMAIL_PROFESSIONAL
Position:                         PLACEHOLDER_POSITION_STR

License:                          GPL v3. You can find it at https://www.gnu.org/licenses/gpl-3.0.en.html
Copyright:                        PLACEHOLDER_COPYRIGHT_STR
Version:                          PLACEHOLDER_APPLICATION_VERSION_STR
Language:                         C++
Framework:                        Qt PLACEHOLDER_QT_VERSION_STR
Source and binary repository:     https://github.com/Folco68/4LN
Changelog:                        https://github.com/Folco68/4LN/commits?author=Folco68

Thanks to Guilhem Barthelemy for original idea and feedback!

How to
======

This program is intended to clone a content into multiple drives (like redistributable USB sticks)
1. Select a source directory (or drag a directory on the program icon, or inside the "Source" box)
2. Select the destinations drives to be cloned, then click the Clone button
3. Check the diff dialog, showing what will be deleted or copied/overwritten
4. Click the Clone button to start the process

/!\ Warning: No consistency check is performed about source and destinations:
/!\
/!\ - you can try to clone a source on itself
/!\ - you can remove an USB stick after the diff is displayed, then start the cloning process
/!\ - you can delete or move the source directory after analysis
/!\ - and certainly many other stupid things
/!\
/!\ => the program won't crash, but you could get into data loss. Please don't complain about your own mistakes


Features
========

- files are deleted if necessary, ignored if the source and destination names match, or copied to the destination if they are not present
- directories are cloned, even if they are empty
- by design, files are overwritten in the destination if they size is < 2Mb. It allows the cloner to modify some small data files and ensure they are cloned, without breaking performance


Under the hood
==============

Written in C++, dynamically built on Windows against Qt 5.12.3
Sources are provided with a CMakeLists.txt file, so you can easily rebuild the program.
See https://www.qt.io/ for infos.

This version separates UI factory and data processing: analyze and clone process are run in external threads,
which allows to keep the UI smooth, usable, while processes are interruptable.
