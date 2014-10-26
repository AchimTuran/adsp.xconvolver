#pragma once

/*	
 *		Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
 *		https://github.com/AchimTuran/LibXConvolver
 *
 *	This file is part of LibXConvolver
 *
 *	LibXConvolver is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	LibXConvolver is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with LibXConvolver.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


class CFilterManager
{
	CFilterManager();
	~CFilterManager();
	
	static
};

// singleton implementation based on 
// http://de.wikibooks.org/wiki/C%2B%2B-Programmierung:_Entwurfsmuster:_Singleton#Normal_.28Heap.29
class N
{
public:
	static N* Get()
	{
		static CGuard g;   // Speicherbereinigung
		if (!_instance)
			_instance = new N();
		
		return _instance;
	}
	void xyz();

private:
	static N* _instance;
	N(); /* verhindert, dass ein Objekt von außerhalb von N erzeugt wird. */

	// protected, wenn man von der Klasse noch erben möchte
	N(const N&); /* verhindert, dass eine weitere Instanz via Kopie-Konstruktor erstellt werden kann */
	~N();
	
	class CGuard
	{
		public:
		~CGuard()
		{
			if(N::_instance)
			{
				delete N::_instance;
				N::_instance = NULL;
			}
		}
	};
	friend class CGuard;
};