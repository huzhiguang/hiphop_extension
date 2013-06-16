<div><h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>1.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>The directory structure</span></h1><p><span>Idl file</span><span style="font-family:宋体">：</span><span>src/idl/xcache.idl.php</span></p>
<p><span>Implement files</span><span style="font-family:宋体">：</span><span>src/runtime/ext/ext_xcache.h</span></p>
<p><span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;src/runtime/ext/ext_xcache.cpp</span></p>
<p><span>Test case:test</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>2.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Support func</span><span>tions</span><span style="font-family:宋体">：</span></h1><p><b><span>Supportfcuntions</span></b><b><span style="font-family:宋体">：</span><span></span></b></p>
<p><span>xcache_set</span></p>
<p><span>xcache_get</span></p>
<p><span>xcache_inc</span></p>
<p><span>xcache_unset</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>3.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Hiphop xcache install way</span><span style="font-family:宋体">：</span></h1><p><span>cp xcache.idl.php $HPHP_HOME/src/idl</span></p>
<p><span>cd $HPHP_HOME/src</span></p>
<p><span>EXT=xcache make –C idl install</span></p>
<p><span>cp ext_xcache.h $HPHP_HOME/src/runtime/ext</span></p>
<p><span>cp ext_xcache.cpp$HPHP_HOME/src/runtime/ext</span></p>
<p><span style="font-size:10.0pt;font-family:&quot">&nbsp;</span></p>
<p><span style="font-size:10.0pt;font-family:&quot">Other ways i</span><span style="font-size:10.0pt;font-family:&quot">n accordance with the HHVM compiler extensions can beadded</span></p>
<p><span>&nbsp;</span></p>
<h1 style="margin-left:21.25pt;text-indent:-21.25pt"><span>4.<span style="font-weight:normal;font-size:7pt;font-family:'Times New Roman'">&nbsp;&nbsp;</span></span><span>Test case</span></h1><p><span style="font-size:10.0pt;font-family:&quot">wait for join</span></p>
</div>

