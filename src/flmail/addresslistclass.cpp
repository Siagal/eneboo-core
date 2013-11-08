/*
    Copyright (C) 2001-2002 by theKompany.com <www.thekompany.com>
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    By PLUGINS we mean the commercial plug-ins developed by theKompany.com
    for Aethera.

    In addition, as a special exception, theKompany.com gives permission
    to link the code of this program with PLUGINS (or with
    modified versions of PLUGINS that use the same license as PLUGINS),
    and distribute linked combinations including the two.  You must obey
    the GNU General Public License in all respects for all of the code used
    other than PLUGINS.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.

    This license grants you the ability to use PLUGINS with Aethera only
    and may not be used outside of Aethera.
    See also http://www.thekompany.com/products/license.txt for details.
*/
/*
 * This file is part of Magellan <http://www.kAlliance.org/Magellan>
 *
 * Copyright (c) 1998-2000 Teodor Mihai <teddy@ireland.com>
 * Copyright (c) 1998-2000 Laur Ivan <laur.ivan@ul.ie>
 * Copyright (c) 1999-2000 Virgil Palanciuc <vv@ulise.cs.pub.ro>
 *
 * Requires the Qt widget libraries, available at no cost at
 * http://www.troll.no/
 *
 * Also requires the KDE libraries, available at no cost at
 * http://www.kde.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <addresslistclass.h>
#include <qtextstream.h>
#include <qstringlist.h>

#include <qmessagebox.h>

#include <qregexp.h>

AddressListClass::AddressListClass(const QString &t, bool unicode)
{
    QStringList s;
    QChar c;
    QString token;
    bool nesting = false;
    QTextIStream txt(&t);

    while (!txt.atEnd()) {
        txt >> c;

        if (c=='"')
            nesting = !nesting;

        if (c==',' && !nesting) {
            if ( !token.isEmpty() ) {
                s.append(token);
                token = QString::null;
            }
        } else
            token += c;
    }

    if ( !token.isEmpty() )
        s.append(token);

    for (QStringList::ConstIterator it=s.begin(); it!=s.end(); ++it) {
        if ( unicode ) {
            addrList.append( AddressClass(*it) );
        } else {
            addrList.append( AddressClass((*it).utf8()) );
        }
    }
}

AddressListClass::AddressListClass(const QCString &t)
{
    QStrList s;
    char c;
    QCString token;
    bool nesting=false;
    QTextStream txt(t, IO_ReadOnly);

    while (!txt.atEnd()) {
        txt.readRawBytes(&c, 1);

        if (c=='"')
            nesting=!nesting;

        if (c==',' && !nesting) {
            if ( !token.isEmpty() ) {
                s.append(token);
                token.truncate(0);
            }
        } else
            token += c;
    }

    if (!token.isEmpty())
        s.append(token);

    for (unsigned i=0;i<s.count();i++)
        addrList.append( AddressClass( QCString(s.at(i)) ) );
}

AddressListClass::AddressListClass() {}

AddressListClass::~AddressListClass() {}

AddressClass AddressListClass::operator[](int index)
{
    return addrList[index];
}

QValueList<AddressClass> AddressListClass::getAddrList()
{
    return addrList;
}

int AddressListClass::count()
{
    return addrList.count();
}

QStringList AddressListClass::toQStringList() const
{
    QStringList t;
    QCString address;

    for (QValueList<AddressClass>::ConstIterator it = addrList.begin(); it!= addrList.end() ; ++it) {
        address = (*it).getOriginalAddress();
        if ( !address.isEmpty() )
            t.append(address);
    }

    return t;
}

AddressListClass::operator QCString() const
{
    QCString t;
    QCString address;

    for (unsigned i=0; i<addrList.count(); i++) {
        address = addrList[i].getOriginalAddress();
        if ( !address.isEmpty() ) {
            if ( !t.isEmpty() )
                t += ", ";
            t += address;
        }
    }

    return t;
}

QCString AddressListClass::toQCString() const
{
    return operator QCString();
}

void AddressListClass::addAddress(AddressClass newAddress)
{
    addrList.append(newAddress);
}

void AddressListClass::clear()
{
    addrList.clear();
}

QString AddressListClass::returnString() const
{
    QString text;
    QString address;

    for (unsigned i=0; i<addrList.count(); i++) {
        address = addrList[i].fullAddress();
        if ( !address.isEmpty() ) {
            if ( !text.isEmpty() )
                text += ", ";
            text += address;
        }
    }

    return text;
}

QStringList AddressListClass::returnNameStringList() const
{
    QStringList list;
    QString address;

    for (unsigned i=0; i<addrList.count() ; i++) {
        address = addrList[i].fullAddress();
        if ( !address.isEmpty() )
            list.append(address);
    }

    return list;
}

QTextStream& operator<<(QTextStream &s, AddressListClass &a)
{
    QCString t = (QCString)a;
    s << t;
    return s;
}