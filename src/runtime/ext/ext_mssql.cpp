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

#include <runtime/ext/ext_mssql.h>
#include <iostream>
using std::cout;
using std::endl;
namespace HPHP {
///////////////////////////////////////////////////////////////////////////////
/*
extern char* mssql_glob_proc;

static MSSQLResult *get_result(CVarRef result) {
    MSSQLResult *res = result.toObject().getTyped<MSSQLResult>
      (!RuntimeOption::ThrowBadTypeExceptions,
	    !RuntimeOption::ThrowBadTypeExceptions);
	if (res == NULL || (res->get() == NULL && !res->isLocalized())) {
		raise_warning("supplied argument is not a valid MSSQL result resource");
	}
	return res;
}
*/

char * mssql_glob_proc;
MSSQL::MSSQL(const char *server, const char *username, const char *password, bool new_link)
{
	if(server) m_server = server;
	if(username) m_username = username;
	if(password) m_password = password;
				
	RETCODE retCode;
	retCode = dbinit();
	if (retCode == FAIL) {
		fprintf(stderr, "dbinit() failed\n");
	}
	loginRec = dblogin();
	if(!loginRec)
	{
		fprintf(stderr, "unable to allocate login structure\n");
	}
	dbmsghandle((MHANDLEFUNC)f_msg_handler);
//	dbprocmsghandle(loginRec, (MHANDLEFUNC) f_msg_handler);
	DBSETLUSER(loginRec, username);
	DBSETLPWD(loginRec, password);
	if ((m_conn = dbopen(loginRec, server)) == NULL)									 
	{
		raise_error("runtime/ext_mssql: dbopen faile\n");
		//fprintf(stderr, "dbopen failed\n");
	}							
//	printf("conn.addr:%p====create Mysql\n",m_conn);
}

MSSQL::~MSSQL()
{ 
  //printf("enter ~MSSQL m_conn:%p\n",m_conn);
  if(!isPersistent){
  	if (m_conn) {
      		m_last_error_set = false;
	  	m_last_errno = 0;
	  	m_last_error.clear();
	  	//printf("~MSSQL m_conn:%p\n",m_conn);
	  	dbclose(m_conn);
	  	m_conn = NULL;
		dbloginfree(loginRec);
	  //printf("mssql connect close!\n");
  	}else{
		dbloginfree(loginRec);
	}	
  //printf("mssql connect have close!\n");
  }
}

MSSQL *MSSQL::Get(CVarRef link_identifier) {
  if (link_identifier.isNull()) {
      return NULL;
  }
  MSSQL *mssql = link_identifier.toObject().getTyped<MSSQL>
	(!RuntimeOption::ThrowBadTypeExceptions,
	 !RuntimeOption::ThrowBadTypeExceptions);
  return mssql;
}

DBPROCESS *MSSQL::GetConn(CVarRef link_identifier, MSSQL **rconn)
{
	MSSQL *mssql = Get(link_identifier);
	DBPROCESS *ret = NULL;
	if(mssql)
	{
		ret = mssql->get();
	}
	if(ret == NULL)
	{
		raise_warning("supplied argument is not a valid MSSQL-link resource");
	}
	if(rconn)
	{
		*rconn = mssql;
	}
	return ret;
}

Variant php_mysql_do_query_general(CStrRef query, CVarRef link_id) 
{
  MSSQL *rconn = NULL;
  DBPROCESS *conn = MSSQL::GetConn(link_id, &rconn);
  if (!conn || !rconn) return false;
	dbfreebuf(conn);
//	if(dbsqlexec(conn) != SUCCEED){
//		raise_notice("runtime/ext_mssql: failed executing [%s]", query.data());
//	}
	if(dbcmd(conn, query.data()) != SUCCEED)
	{
		raise_notice("runtime/ext_mssql: failed executing [%s]", query.data());
		return false;
	}
	if(dbsqlsend(conn) != SUCCEED)
	{
		raise_notice("runtime/ext_mssql: failed executing [%s]", query.data());
		return false;
	}

	if(dbsqlok(conn) != SUCCEED)
	{
		raise_notice("Waring: [%p]",mssql_glob_proc);
		raise_notice("runtime/ext_mssql: failed executing [%s]", query.data());
		return false;
	}

	int64 tt = DBCOUNT(conn);
	if(tt < 0 ){
	c_MssqlResult *result = NEWOBJ(c_MssqlResult)();
	result->cursor = 0;
	while(dbresults(conn) != NO_MORE_RESULTS){
		int row_code ;
		Array allrow = Array::Create();
		int num_fields = dbnumcols(conn);
		int64 row_cousor = 0;
		//dbsetopt(conn,DBBUFFER,NULL,1000);
		//row_code = dbsetrow(conn,1);
		while((row_code = dbnextrow(conn)) != NO_MORE_ROWS){
			Array resultrow = Array::Create();
			//Array *resultrow1=new Array();
			//Array resultrow=*resultrow1;
			for(int c = 0 ; c < num_fields ; c++){
				BYTE * pdata_new = dbdata(conn, c+1);
				size_t data_len = dbdatlen(conn, c+1);
				std::string std_fname = (char *)dbcolname(conn,c+1);
				if(pdata_new == NULL && data_len == 0){
					resultrow.set(Variant(std_fname),Variant());
					continue;
				//	cout <<(char *)dbcolname(conn,c+1) << "---" <<  endl;
				}
				BYTE * pdata=NULL;
				pdata = (BYTE*)calloc(data_len+1,sizeof(BYTE));
				if(NULL == pdata){
					raise_notice("calloc memory address failed!");
					return rconn;
				}
				memcpy(pdata,pdata_new,data_len);

				result->resList.push_back(pdata);				

//				std::string std_fname = (char *)dbcolname(conn,c+1);
				//cout<< "key:"<<std_fname<<endl;
				int type = dbcoltype(conn,c+1);
				if(data_len != 0){
					switch(type){
						case SYBBINARY:
						case SYBVARBINARY:
							 break;
						case SYBCHAR:
						case SYBVARCHAR:
							 //cout <<"it is char:" << pdata << endl;
							resultrow.set(Variant(std_fname),Variant((char *)pdata));
							 break;
						case SYBTEXT:
							 //cout << "text is :" << pdata << endl;
							 resultrow.set(Variant(std_fname),Variant((char *)pdata));
							 break;
						case SYBDATETIME:
						case SYBDATETIME4:
						case SYBDATETIMN:
							 //cout << "datatime: " << "type is:" << type<< endl;
							 //data_len += 5;
							 //char * res_buf;
							 //DBDATEREC dateinfo;
							 //res_buf = (unsigned char *) emalloc(data_len+1);
							 //data_len = dbcovert(NULL,type,pdata,data_len,SQLCHAR,res_buf,-1);
							 //dbdatecrack(conn,&dateinfo,(DBDATETIME *)pdata);
							 //sprintf(res_buf, "%d-%02d-%02d %02d:%02d:%02d" , dateinfo.year, dateinfo.month, dateinfo.day, dateinfo.hour, dateinfo.minute, dateinfo.second);
							 //cout << "datatime==:" << "value is=" << res_buf << endl;
							 //resultrow.set(Variant(std_fname),Variant((char *)res_buf));
							 break;
						case SYBDECIMAL:
						case SYBREAL:
						case SYBFLT8:
						case SYBFLTN:
							 break;
						case SYBINT1:
						case SYBINT2:
						case SYBINT4:
						case SYBINTN:
							 //cout <<"it is int:" << *(DBINT *)pdata << "type is:" << type << endl;
							 resultrow.set(Variant(std_fname),Variant(*(int *)pdata));
							 break;
						case SYBNUMERIC:
							 break;
						case SYBMONEY:
						case SYBMONEY4:
						case SYBMONEYN:
							 break;
						case SYBBIT:
							 //resultrow.set(Variant(std_fname),Variant((int)pdata));
							 break;
						case SYBIMAGE:
							 break;
						default:
							 //cout << "other" << endl;
							 break;
					}//end switch
				}else{
					resultrow.set(Variant(std_fname),Variant((char *)pdata));
					//switch(type){
					//	case SYBCHAR:
					//		cout <<"it is null char:" << pdata << endl;
					//		resultrow.set(Variant(std_fname),Variant((char *)pdata));
					//		break;
					//	case SYBVARCHAR:
					//		cout <<"it is null varchar:" << pdata << endl;
					//		resultrow.set(Variant(std_fname),Variant((char *)pdata));
					//		break;
					//	default:
					//		cout << "it is null char" << endl;
					//		resultrow.set(Variant(std_fname),Variant());
					//		break;
					//}	
				//	resultrow.set(Variant(std_fname),Variant((char *)pdata));
					//return FALSE;
				}//end switch if
			}//end for

			allrow.set(row_cousor,resultrow);
			++row_cousor;

		}//end while

		result->v_result = allrow;
		rconn->cmr = result;
	}//else{
	//	return false;
	//}//end if
	}
	return rconn;
}

Variant f_mssql_connect(CStrRef server /* = null_string */, CStrRef username /* = null_string */, CStrRef password /* = null_string */, bool new_link /* = false */) {
	Object ret;
	MSSQL *msSQL = new MSSQL(server.c_str(), username.c_str(), password.c_str(), false);
	msSQL->isPersistent = false;
	msSQL->cmr = NULL;
	//MSSQL *msSQL = NEWOBJ(MSSQL)(server.c_str(), username.c_str(), password.c_str(), false);
	ret = msSQL;
	return ret;
}

const char* Mssql_PersistentObject_Name="Mssql_Conn";
Variant f_mssql_pconnect(CStrRef server /* = null_string */, CStrRef username /* = null_string */, CStrRef password /* = null_string */, bool new_link /* = false */) {
//	throw NotImplementedException(__func__);
//	Object ret;
	MSSQL *msSQL = NULL;
	msSQL= dynamic_cast<MSSQL*>(g_persistentObjects->get(Mssql_PersistentObject_Name, server));
	bool flag = false;
	if(msSQL != NULL){
		DBPROCESS  *m_msSQL = msSQL->m_conn;
		if(dbdead(m_msSQL) == TRUE ){
			dbclose(m_msSQL);//close current link
			msSQL = new MSSQL(server.c_str(), username.c_str(), password.c_str(), false);
			msSQL->isPersistent = true;
			flag = true;
		}

	}else{
		msSQL = new MSSQL(server.c_str(), username.c_str(), password.c_str(), false);
		msSQL->isPersistent = true;
		flag = true;
	}
	if(flag){
		g_persistentObjects->set(Mssql_PersistentObject_Name,server, msSQL);
	}
	return msSQL;
}

bool f_mssql_close(CVarRef link_identifier /* = null */) {
 	MSSQL *mssql = NULL;
	MSSQL::GetConn(link_identifier, &mssql);
	//printf("enter mssql close\n");
	//printf("first :mssql->m_conn:%p\n",mssql->m_conn);
	if(!mssql->isPersistent){
		if(mssql->m_conn){
	    		mssql->m_last_error_set = false;
	    		mssql->m_last_errno = 0;
	    		mssql->m_last_error.clear();
	 		dbclose(mssql->m_conn);
	 		mssql->m_conn = NULL;
			//dbloginfree(mssql->loginRec);
	// printf("end :mssql->m_conn:%p\n",mssql->m_conn);
		}
	}
	//printf("have destroy mysql object\n");
	return true;
}

bool f_mssql_select_db(CStrRef database_name /* = null */, CVarRef link_identifier /* = null */) {
  DBPROCESS *conn = MSSQL::GetConn(link_identifier);
  if(conn){
  if(dbuse(conn, database_name.c_str()) == SUCCEED)
  {
	  return true;
  }
  else
  {
	  return false;
  }
  }else{
	raise_error("mssql_select_db function conn is NULLPOINT!\n");	
	return false;
  }
}

Variant f_mssql_query(CStrRef query /* = null */, CVarRef link_identifier /* = null */,int64 batch_size /* = 0 */) {
	return php_mysql_do_query_general( query, link_identifier);
}

Variant f_mssql_num_rows(CVarRef result /* = null */) {
	int64 nrows = 0;
	MSSQL *rconn = NULL;
	DBPROCESS *conn = MSSQL::GetConn(result, &rconn);
	if (!conn || !rconn) return false;

	c_MssqlResult *msqlresult = rconn->cmr;
//	nrows = DBCOUNT(conn);

//	if(nrows < 0 ){
		Array resultList = msqlresult->v_result;
		if(!resultList.empty()){
			nrows = resultList.size();
			if( nrows > 0 ){
				return nrows;
			}else{
				return 0;
			}
		}else{
			return "";
		}
//	}else{
//		raise_error("mssql_num_rows expects parameter 1 to be resource.");	
//		return "";
//	}
}

Variant f_mssql_rows_affected(CVarRef result /* = null */) {
//  throw NotImplementedException(__func__);
    MSSQL *rconn = NULL;
	int nrows = 0;
	DBPROCESS *conn = MSSQL::GetConn(result, &rconn);
	if (!conn || !rconn) return false;

	if(dbresults(conn) != NO_MORE_RESULTS){
		nrows = DBCOUNT(conn);
		if (nrows > -1){
            return nrows;
        }else{
            return Variant(0);
		}
	}else{
		c_MssqlResult *msqlresult = rconn->cmr;
		Array resultList = msqlresult->v_result;
		if(!resultList.empty()){
			nrows = resultList.size();
			if( nrows > 0 ){
				return nrows;
			}else{
				return 0;
			}   
		}else{
			return ""; 
		}   
	}
}

Variant f_mssql_fetch_row(CVarRef result /* = null */) {
	Array ret;
	Array rearray;
	//int ncols = 0;
	//int row_code = 0;
	//int c = 0;
	//BYTE *pdata = NULL;
	//size_t data_len = 0;
	MSSQL *rconn = NULL;
	DBPROCESS *conn = MSSQL::GetConn(result, &rconn);
	if (!conn || !rconn) return false;

	c_MssqlResult *msqlresult = rconn->cmr;
	Array resultList = msqlresult->v_result;
	int cur_row = msqlresult->cursor;
	int totalnum = resultList.size();

	if(!resultList.isNull()){
			if( cur_row < totalnum ){
					Array ret = resultList[msqlresult->cursor];
					int c=0;
					for( ArrayIter iter(ret) ; iter ; ++iter){
						rearray.set(c,iter.second());
						++c;
					}
					++msqlresult->cursor;
					return rearray;
			}else{
					return false;
			}
	}else{
			return false;
	}


//	if(dbresults(conn) != NO_MORE_RESULTS)
//	{
//		if((row_code = dbnextrow(conn)) != NO_MORE_ROWS)
//		{
//			ncols = dbnumcols(conn);
//			for(c = 0; c < ncols; c++)
//			{
//				pdata = dbdata(conn, c+1);
//				data_len = dbdatlen(conn, c+1);
//				if(data_len != 0)
//					ret.set(c, (char *)pdata);
//				else
//					return FALSE;
//			}
//		}
//		else
//			return FALSE;
//	}
//	else
//		return FALSE;

	return ret;	
}

c_MssqlResult::c_MssqlResult(const ObjectStaticCallbacks *cb):ExtObjectData(cb){
	    
}

c_MssqlResult::~c_MssqlResult(){
	for(ResList::iterator itr = resList.begin();itr != resList.end(); ++itr ){
		BYTE * pdata = *itr;
		free(pdata);
		pdata = NULL;
	}
}

void c_MssqlResult::t___construct(){
  INSTANCE_METHOD_INJECTION_BUILTIN(MssqlResult, MssqlResult::__construct);
}

String c_MssqlResult::t___tostring(){
  INSTANCE_METHOD_INJECTION_BUILTIN(MssqlResult, MssqlResult::__tostring);
  return "";	    
}


Variant f_mssql_fetch_assoc(CVarRef result /* = null */) {
	MSSQL *rconn = NULL;
    DBPROCESS *conn = MSSQL::GetConn(result, &rconn);
	if (!conn || !rconn) return false;  

    c_MssqlResult *msqlresult = rconn->cmr;
	Array resultList = msqlresult->v_result;
	int cur_row = msqlresult->cursor;
	//cout<< cur_row << endl;
	int totalnum = resultList.size();
	//cout<<totalnum<<endl;
	if(!resultList.isNull()){
		if( cur_row < totalnum ){
			Array ret = resultList[msqlresult->cursor];
			++msqlresult->cursor;	
			return ret;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

Variant f_mssql_free_result(CVarRef result /* = null */) {
//	throw NotImplementedException(__func__);
	MSSQL *rconn = NULL;
	//RETCODE retCode;
	int retvalue;
	DBPROCESS *conn = MSSQL::GetConn(result,&rconn);
	if(!conn || !rconn) return false;

	do{
		dbcanquery(conn);
		//retCode = dbcanquery(conn);
		//printf("has free mem num ==%d",retCode);
		//cout << "has free mem num "<<endl;
		//cout << retCode <<endl;
		retvalue = dbresults(conn);	
	}while(retvalue == SUCCEED);	

	return true; 
}

Variant f_mssql_get_last_message() {
//  throw NotImplementedException(__func__);
	if(mssql_glob_proc){
		printf("%p",mssql_glob_proc);
		return mssql_glob_proc;
	}else{
		return "there is no message";
	}
}

int f_msg_handler(DBPROCESS * dbproc, int msgno, int msgstate, int severity, char *msgtext, char *srvname, char *procname, int line){
	enum {changed_database = 5701, changed_language = 5703 };
	if (msgno == changed_database || msgno == changed_language)
		return 0;
	if ( msgno > 0 ){
		fprintf(stderr, "Msg %ld, Level %d, State %d\n", (long) msgno, severity, msgstate);

		if(strlen(srvname) > 0){
			fprintf(stderr, "Server '%s', ", srvname);
		}
//		if(strlen(procname) > 0){
//			fprintf(stderr, "Procedure '%s', ", procname);
//		}
		if(line > 0){
			fprintf(stderr, "Line %d", line);
		}
	}
	fprintf(stderr, "message:%s\n", msgtext);
	if(severity > 10){
		fprintf(stderr, "error: severity %d \n",  severity);
//		exit(severity);
	}
	mssql_glob_proc = msgtext;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
}
