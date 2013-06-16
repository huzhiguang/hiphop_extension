/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include <runtime/ext/ext_xcache.h>
#include <runtime/base/shared/concurrent_shared_store.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

static XCacheStore s_xcache_store ;

XCacheStore::XCacheStore() {
	m_store = new ConcurrentTableSharedStore(0) ;
}

XCacheStore::~XCacheStore() {
	if (m_store) {
		delete m_store ;
		m_store = NULL ;
	}
}

SharedStore *XCacheStore::getShareStore() {
	return m_store ;
}



bool f_xcache_set(CStrRef name, CVarRef value, int ttl /* = 0 */) {
	return s_xcache_store.getShareStore()->store( name, value, ttl, true) ;
}

Variant f_xcache_get(CStrRef name) {
	Variant v ;
	if (s_xcache_store.getShareStore()->get(name, v)) {
		return v ;
	} else {
	  	v = false;
	}
	return v;
}


int64 f_xcache_inc(CStrRef key, int64 value,/* = 1*/ int ttl /* = 0 */) {
	Variant v ;
	if (s_xcache_store.getShareStore()->get(key, v)) {
		bool found = false ;
		int64 newValue = s_xcache_store.getShareStore()->inc( key, value, found) ;
		if (newValue < 0) {
			newValue = 0 ;
			s_xcache_store.getShareStore()->store( key, newValue, ttl, true) ;
		}
		return newValue ;
	} else {
		s_xcache_store.getShareStore()->store( key, value, ttl, true) ;
		return value ;
	}
}

bool f_xcache_unset(CStrRef name) {
	return s_xcache_store.getShareStore()->erase(name) ;
}


///////////////////////////////////////////////////////////////////////////////
}
