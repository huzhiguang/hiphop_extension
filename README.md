<div><h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>1.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>The directory structure</span></h1><p><span>Idl file</span><span style="font-family:宋体">：</span><span>src/idl/mongodb.idl.php</span></p>
<p><span>Implement files</span><span style="font-family:宋体">：</span><span>src/runtime/ext/ext_mongodb.h</span></p>
<p><span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;src/runtime/ext/ext_mongodb.cpp</span></p>
<p><span>Test case:test</span></p>
<p><span>Support_package: support_package/ mongodb-mongo-c-driver-17ff925.20130318.tar.bz2</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>2.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Support func</span><span>tions</span><span style="font-family:宋体">：</span></h1><p><b><span>SupportClass</span></b><b><span style="font-family:宋体">：</span><span></span></b></p>
<p><span>Mongo</span></p>
<p><span>MongoDB</span></p>
<p><span>MongoCollection</span></p>
<p><span>MongoCursor</span></p>
<p><span>MongoDate</span></p>
<p><b><span>Supportfcuntions</span></b><b><span style="font-family:宋体">：</span><span></span></b></p>
<p><span>Detailed see mongodb.idl.php</span></p>
<p><b><span>&nbsp;</span></b></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>3.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>mongodb <span>Support pack</span>age installation</span><span style="font-family:宋体">：</span></h1><p><span style="font-size:10.0pt;font-family:&quot">The original</span><span style="font-size:10.0pt;font-family:&quot"> git address</span><span style="font-family:宋体">：</span></p>
<p><span><a href="https://github.com/mongodb/mongo-c-driver">https://github.com/mongodb/mongo-c-driver</a></span></p>
<p><span>&nbsp;</span></p>
<p><span>patch version</span><span style="font-family:宋体">：</span></p>
<p><span>mongodb-mongo-c-driver-17ff925.20130318.tar.bz2</span></p>
<p><span>patch content</span><span style="font-family:宋体">：</span></p>
<p><span>mongodb connect timeout function</span></p>
<p><span>mongodb connect pool function</span></p>
<p><span>&nbsp;</span></p>
<p><span>tar jxvf mongodb-mongo-c-driver-17ff925.20130318.tar.bz2–C mongodb-c-driver</span></p>
<p><span>make</span></p>
<p><span>cd&nbsp; mongodb-c-driver</span></p>
<p><span>cp &nbsp;src/mongo/mongo.h&nbsp; &nbsp;$CMAKE_PREFIX_PATH/include/mongodb_c_driver/mongo.h</span></p>
<p><span>cp &nbsp;ibmongoc.so&nbsp; &nbsp;$CMAKE_PREFIX_PATH/lib/mongodb_c_driver/</span></p>
<p><span>&nbsp;</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>4.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Hiphop mongodb install way</span><span style="font-family:宋体">：</span></h1><p><span>edit $HPHP_HOME/CMake/HPHPFind.cmake</span></p>
<p><span>add content:</span></p>
<p><span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; target_link_libraries(${target}/export/dev_hhvm/usr/lib/mongodb_c_driver/libmongoc.so)</span></p>
<p><span>save this file</span></p>
<p><span>&nbsp;</span></p>
<p><span>cp mongodb.idl.php $HPHP_HOME/src/idl</span></p>
<p><span>cd $HPHP_HOME/src</span></p>
<p><span>EXT=mongodb make –C idl install</span></p>
<p><span>cp ext_mongodb.h $HPHP_HOME/src/runtime/ext</span></p>
<p><span>cp ext_mongodb.cpp$HPHP_HOME/src/runtime/ext</span></p>
<p><span>&nbsp;</span></p>
<p><span>&nbsp;</span></p>
<p><span style="font-size:10.0pt;font-family:&quot">Other ways i</span><span style="font-size:10.0pt;font-family:&quot">n accordance with the HHVM compiler extensions can beadded</span></p>
<p><span>&nbsp;</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>5.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Test case</span></h1><p><span style="font-size:10.0pt;font-family:&quot">wait for join</span></p>
</div>

