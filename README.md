# Paper Blossoms
A character generator for Legend of the Five Rings.

Paper Blossoms is an open source character generator for Legend of the Five Rings, a roleplaying game by Fantasy Flight Games.  The application and author(s) are not affiliated with FFG or any other official L5R party.  Legend of the Five Rings and all associated content is property of Fantasy Flight Games.

## Downloading and Using Paper Blossoms
Caution:
As always, this software is provided without warranty, and the author assumes no responsibility or liability for its use.

The application can be downloaded [here](https://github.com/dashnine/PaperBlossoms/releases).

This application is a platform-native application, and does not use or need network connections.  Code should be buildable and runnable on Windows, OSX, and Linux. Testing and releases are currently conducted on Windows 10 and OSX.

## Data
Much like the excellent [Star Wars character generator by OggDude](http://www.legendsofthegalaxy.com/Oggdude/) this application does not include descriptions; you are expected to own and use the books while creating or editing your character.  If you own the books and would like to enter the descriptions yourself, this functionality is supported by editing the _user_descriptions_ in the database.  Note that in early releases, databases schemas may change; efforts will be made to provide assistance in migrating exported data in new releases if/when this occurs.

This application utilizes a Sqlite database to store key data such as schools, clans, and families; this database is copied on run to a platform-dependent application directory (e.g. the AppData/Local/PaperBlossoms directory on Windows), and a warning will appear whenever the application's bundled data file is older than the local copy (or the local copy is missing). 
If you would like to add custom data ('cause who doesn't want to be a Cat clan shinobi, eh?) you can do so by editing the *user_* tables in the database--for now, using a tool such as [DB Browser for SQLite](https://sqlitebrowser.org/).

Basic export and import functionality have been provided in the tools menu.  By selecting *Export User Data Tables...* and choosing a folder, you can dump the user_ tables to that folder.  Selecting *Import User Data Tables...* will attempt to import these CSVs.  _This is limited functionality provided for convenience, and may render your DB unusuable in the event of a problem or schema change. Always back up your data if you're concerned about this._  If you enter custom data, be sure to export it to save your work.

## Contributing
Paper Blossoms is written in C++, with Qt5. Application data is stored in a sqlite database, and accessed exclusively through the interface provided in dataaccesslayer.cpp -- any new queries to the data should be made there.  Save files have version numbers associated; going forward, modifications will attempt to use these version numbers to check and handle save file compatibility.  I'm happy to accept and review pull requests, and will be populating tasks and feature requests on this github page!  A general overview of the code is in the wiki, and we hope to have more added in the near future.

If you would like to make a contribution to the data, consult the .json files stored in the data folder of the source.  For the sake of track-ability, we're making edits there, and then generating the paperblossoms.db file with the included python scripts.

If you would like to contribute a translation of the UI, you'll need to modify a .ts file for your locale.  I've added notes [here](https://github.com/dashnine/PaperBlossoms/wiki/Translation).

## Credits
Data modeling and data entry ninja-provided by [@meow9th](https://github.com/meow9th).

L5R and all associated data is owned by FFG.  You should go buy the books [here](https://www.drivethrurpg.com/browse/pub/6/Fantasy-Flight-Games/subcategory/36_28812/Legend-of-the-Five-Rings-5th-Edition), [here](https://www.fantasyflightgames.com/en/legend-of-the-five-rings-roleplaying-game/), or at your friendly local game store. 

## Additional Notes:
This application is created using Qt 5, which is licensed under the [LGPLv3](https://doc.qt.io/qt-5/lgpl.html).  A copy of this license is also available in the application directory.  While this application is distributed with binaries for Qt, any compatible Qt binaries may be used in place of those distributed, limited only by version incompatibilities. Sakura imagery on the splashscreen is from http://pngimg.com/download/49821, and is Licensed under Creative Commons: https://creativecommons.org/licenses/by-nc/4.0/ . The icon is a modified version of "sakura" by Pham Duy Phuong Hung from the Noun Project ( https://thenounproject.com/term/sakura/1565575/ ), which is also CC licenced.

Ring images are property of FFG, and were originally created for the original Legend of the Five Rings card game by AEG (1995). The assets used here were taken from www.imperialadvisor.com and modified to work with the application; these can be removed immediately upon request by any owning party. 


### Thanks and Contributor List
Thanks to our users, bug reporters, contributors, well wishers, and (of course) to the folks that have produced this excellent game (current AND prior editions)!

Since Github is not always good about listing who has helped out on a project, I wanted to do so.  The following handles have contributed to Paper Blossoms -- thanks for your contributions!
* dashnine
* meow9th
* aajabrams
* OpenNinjia
* ruronin (French UI Translation)
* Albertorius (Spanish UI Translation)
