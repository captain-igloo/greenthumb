Greenthumb - Cross-platform GUI for Betfair
===========================================

Introduction
------------
Sports betting exchange [Betfair](www.betfair.com) provides an API that allows you to:
 * read market information and prices
 * place / update / cancel bets
 * manage your account

and more.  Greenthumb is a wrapper for this API and can be used as an alternative to the Betfair website.  It is written in C++ and uses the wxWidgets toolkit.

Screenshots
-----------

![Screenshot](https://raw.githubusercontent.com/captain-igloo/greenthumb/master/doc/screenshot1.png)

License
-------

Greenthumb is released under the MIT license. See the LICENSE file for the full text.

Requirements
------------

* [wxWidgets](https://www.wxwidgets.org)
* [greentop](https://github.com/captain-igloo/greentop)
* [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
* [libcurl](http://curl.haxx.se/libcurl/)
* [SOCI](http://soci.sourceforge.net/)
* [SQLite](https://www.sqlite.org/)

Build
-----

./configure --prefix=/usr

make

make install

Contact
-------

colindoig [at] gmail [dot] com

-- Colin Doig
