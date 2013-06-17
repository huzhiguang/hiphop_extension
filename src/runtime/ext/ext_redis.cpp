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

#include <runtime/ext/ext_redis.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

c_Redis::c_Redis(const ObjectStaticCallbacks *cb) :
		ExtObjectData(cb) {
}
c_Redis::~c_Redis() {
	printf("c_Redis::~c_Redis()..... _redisContext->err = %d\n", _redisContext->err) ;
	if (_redisContext != NULL && !_redisContext->err) {
 	//if (_redisContext != NULL ) {
		redisFree(_redisContext) ;
		_redisContext = NULL ;
	}
}

void c_Redis::t___construct() {
  INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::__construct);
  	return ;
}

bool c_Redis::t_connect(CStrRef host, int port, double timeout) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::connect);
  	return t_pconnect(host, port, timeout) ;
}

bool c_Redis::t_pconnect(CStrRef host, int port, double timeout) {
  	INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::pconnect);
  	int iSec = (int)timeout ;
	float fUsec = timeout - iSec ;
	int iUsed = int(fUsec*1000000) ;
	struct timeval oTimeout = {iSec, iUsed };

	const char *pHost = host.empty() ? NULL : host.c_str();
	_redisContext  = redisConnectWithTimeout(pHost, port, oTimeout);
	if (_redisContext->err) {
		return false ;
	} else {
		return true ;
	}
}

bool c_Redis::t_select(int dbindex) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::select);
  	if (_redisContext != NULL && !_redisContext->err) {
		reply = (redisReply *)redisCommand(_redisContext,"SELECT %d", dbindex);
		if (NULL != reply) {
			char * pReplyStr = reply->str ;
			int iCmpRst = strcmp(pReplyStr, "OK") ;
			freeReplyObject(reply);
			if (0 == iCmpRst) {
				return true ;
			} else {
				 return false ;
			}
		}
	}
	return false ;
}

Variant c_Redis::t_get(CStrRef key) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::get);
  	if (0 == key.size() ) {
		return false ;
	}
	Variant obj = false ;
	if (_redisContext != NULL && !_redisContext->err) {
		string sCommand = "GET";
		sCommand.append(" ") ;
		sCommand.append(key->data()) ;
		redisReply *reply = (redisReply *)redisCommand(_redisContext, sCommand.c_str()) ;

		int iLen = reply->len;
		if ( iLen > 0 ) {
			std::string str(reply->str) ;
			obj =  str ;
		}
		freeReplyObject(reply);
	}
	return obj ;
}
Variant c_Redis::t_mget(CArrRef keys) {
  INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::mget);
  	Array returnValue;
	if (_redisContext != NULL && !_redisContext->err) {
		string sCommand = "MGET";
		for (ArrayIter iter(keys) ; iter ; ++iter) {
			Variant vKey = iter.second();
			if (!vKey.isString()) continue;
			StringData *key = vKey.getStringData();
			if (key->empty()) continue;
			sCommand.append(" ") ;
			sCommand.append(key->data()) ;
		}

		redisReply *reply = (redisReply *)redisCommand(_redisContext, sCommand.c_str()) ;

		int i ;
		int iLen = (int)reply->elements ;
		Array item ;
		for (i = 0 ; i < iLen ; i++) {
			char* pStr = reply->element[i]->str ;
			if (NULL == pStr) {
				returnValue.append("");
			} else {
				std::string str(pStr) ;
				returnValue.append(str);
			}
		}
		freeReplyObject(reply);
	}
	return returnValue ;
}

Variant c_Redis::t_close() {
  	INSTANCE_METHOD_INJECTION_BUILTIN(Redis, Redis::close);
  	//if (_redisContext != NULL && !_redisContext->err) {
  	if (_redisContext != NULL ) {
		redisFree(_redisContext) ;
		_redisContext = NULL ;
	}
	return true ;
}

///////////////////////////////////////////////////////////////////////////////
}
