<h1>Hiphop mssql extension:</h1>
<div><h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>1.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>The directory structure</span></h1><p><span>Idl file</span><span style="font-family:宋体">：</span><span>src/idl/mssql.idl.php</span></p>
<p><span>Implement files</span><span style="font-family:宋体">：</span><span>src/runtime/ext/ext_mssql.h</span></p>
<p><span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;src/runtime/ext/ext_mssql.cpp</span></p>
<p><span>Test case:test</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>2.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Support func</span><span>tions</span><span style="font-family:宋体">：</span></h1><p><b><span style="font-size:10.0pt;font-family:&quot">Support type</span></b><b><span style="font-size:10.0pt;font-family:&quot">s</span></b><b><span style="font-family:宋体">：</span><span></span></b></p>
<p><span>CHAR</span></p>
<p><span>VARCHAR</span></p>
<p><span>TEXT</span></p>
<p><span>INT</span></p>
<p><span>INT2</span></p>
<p><span>INT4</span></p>
<p><span>INTN</span></p>
<p><span>&nbsp;</span></p>
<p><b><span>Supportfunctions</span></b><b><span style="font-family:宋体">：</span><span></span></b></p>
<p><span>mssql_connect</span></p>
<p><span>mssql_connect</span></p>
<p><span>mssql_close</span></p>
<p><span>mssql_select_db</span></p>
<p><span>mssql_query</span></p>
<p><span>mssql_num_rows</span></p>
<p><span>mssql_rows_affected</span></p>
<p><span>mssql_fetch_row</span></p>
<p><span>mssql_fetch_assoc</span></p>
<p><span>mssql_free_result</span></p>
<p><span>mssql_get_last_message</span></p>
<p><span>msg_handler</span></p>
<p><span>&nbsp;</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>3.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>mssql <span>Support pack</span>age installation</span><span style="font-family:宋体">：</span></h1><p><span>Support mssql library is freetds</span></p>
<p><span>Download support library and install</span></p>
<p><span><a href="http://freetds.schemamania.org/software.html">http://freetds.schemamania.org/software.html</a></span></p>
<p><span>&nbsp;</span></p>
<p><span>cp –r $FREETDS_HOME/lib $CMAKE_PREFIX_PATH/lib/freetds</span></p>
<p><span>cp –r $FREETDS_HOME/include $CMAKE_PREFIX_PATH/include/freetds</span></p>
<p><span>&nbsp;</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>4.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Hiphop mssql install way</span><span style="font-family:宋体">：</span></h1><p><span>edit $HPHP_HOME/CMake/HPHPFind.cmake</span></p>
<p><span>add content:</span></p>
<p><span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; include_directories(/export/dev_hhvm/usr/include/freetds)</span></p>
<p style="text-indent:21.0pt"><span>target_link_libraries(${target}/export/dev_hhvm/usr/lib/freetds/libsybdb.so)</span></p>
<p><span>save this file</span></p>
<p><span>&nbsp;</span></p>
<p><span>cp mssql.idl.php $HPHP_HOME/src/idl</span></p>
<p><span>cd $HPHP_HOME/src</span></p>
<p><span>EXT=mssql make –C idl install</span></p>
<p><span>cp ext_mssql.h $HPHP_HOME/src/runtime/ext</span></p>
<p><span>cp ext_mssql.cpp $HPHP_HOME/src/runtime/ext</span></p>
<p><span>&nbsp;</span></p>
<p><span>&nbsp;</span></p>
<p><span style="font-size:10.0pt;font-family:&quot">Other ways i</span><span style="font-size:10.0pt;font-family:&quot">n accordance with the HHVM compiler extensions can beadded</span></p>
<p><span>&nbsp;</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>5.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Test case</span></h1><p><span style="font-size:10.0pt;font-family:&quot">wait for join</span></p>
</div>
