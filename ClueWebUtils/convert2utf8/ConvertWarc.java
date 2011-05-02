
/*
Lemur License Agreement

  Copyright (c) 2000-2011 The Lemur Project.  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

  3. The names "Lemur", "Indri", "University of Massachusetts" and
     "Carnegie Mellon" must not be used to endorse or promote products
     derived from this software without prior written permission. To
     obtain permission, contact license@lemurproject.org

  4. Products derived from this software may not be called "Lemur" or "Indri"
     nor may "Lemur" or "Indri" appear in their names without prior written
     permission of The Lemur Project. To obtain permission,
     contact license@lemurproject.org.

  THIS SOFTWARE IS PROVIDED BY THE LEMUR PROJECT AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

import java.nio.charset.Charset;
import java.util.regex.*;

import java.io.*;
import java.lang.Integer;
import java.lang.System;
import java.util.*;
import java.util.Hashtable;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;
import org.xml.sax.InputSource;
import java.lang.Exception;

// usage java ConvertWarc path/to/warc/file path/to/output/file
public class ConvertWarc {

  /**
   * Given a <code>byte[]</code> representing an html file of an 
   * <em>unknown</em> encoding,  read out 'charset' parameter in the meta tag   
   * from the first <code>CHUNK_SIZE</code> bytes.
   * If there's no meta tag for Content-Type or no charset is specified,
   * <code>null</code> is returned.  <br />
   * FIXME: non-byte oriented character encodings (UTF-16, UTF-32)
   * can't be handled with this. 
   * We need to do something similar to what's done by mozilla
   * (http://lxr.mozilla.org/seamonkey/source/parser/htmlparser/src/nsParser.cpp#1993).
   * See also http://www.w3.org/TR/REC-xml/#sec-guessing
   * <br />
   *
   * @param content <code>byte[]</code> representation of an html file
   */

  private static final int WRITEOUT_RECORD_EVERY = 100000;


  private static final int CHUNK_SIZE = 2000;
  private static Pattern metaPattern =
    Pattern.compile("<meta\\s+([^>]*http-equiv=\"?content-type\"?[^>]*)>",
		    Pattern.CASE_INSENSITIVE);
  private static Pattern charsetPattern =
    Pattern.compile("charset=\\s*\"*\'*([a-z][_\\-0-9a-z]*)",
		    Pattern.CASE_INSENSITIVE);
  private static Pattern charsetEqualOnly =
    Pattern.compile("charset=",
		    Pattern.CASE_INSENSITIVE);
    
  private static String sniffCharacterEncoding(byte[] content) {
    int length = content.length < CHUNK_SIZE ? 
      content.length : CHUNK_SIZE;
	

    // We don't care about non-ASCII parts so that it's sufficient
    // to just inflate each byte to a 16-bit value by padding. 
    // For instance, the sequence {0x41, 0x82, 0xb7} will be turned into 
    // {U+0041, U+0082, U+00B7}. 
    String str = "";
    try {
      str = new String(content, 0, length,
		       Charset.forName("ASCII").toString());
    } catch (UnsupportedEncodingException e) {
      // code should never come here, but just in case... 
      return null;
    }
	
    Matcher metaMatcher = metaPattern.matcher(str);
    String encoding = null;
    if (metaMatcher.find()) {
      Matcher charsetMatcher = charsetPattern.matcher(metaMatcher.group(1));
      if (charsetMatcher.find()) 
	encoding = new String(charsetMatcher.group(1));
    }
	
    return encoding;
  }
    
  private static HashMap<String, String> encodingmapper =  new HashMap<String, String>();
 
  public static void LoadSpecialCaseEncoding() {
  
    /*
      This are special case encodings which handles various typos and encoding aliases used
      in the dataset.
    */
    encodingmapper.put("ascii",      "ISO-8859-1");
    encodingmapper.put("ansi",       "ISO-8859-1");
    encodingmapper.put("en",         "ISO-8859-1");
    encodingmapper.put("iso",        "ISO-8859-1");
    encodingmapper.put("iso85591",   "ISO-8859-1");
    encodingmapper.put("iso8859",    "ISO-8859-1");
    encodingmapper.put("iso88691",   "ISO-8859-1");
    encodingmapper.put("latin1",     "ISO-8859-1");
    encodingmapper.put("ksc5601",    "EUC-KR");
    encodingmapper.put("latin5",     "ISO-8859-9");
    encodingmapper.put("macintosh",  "x-MacRoman");
    encodingmapper.put("sjis",       "Shift_JIS");
    encodingmapper.put("unicode",    "UTF-8");
    encodingmapper.put("utf",        "UTF-8");
    encodingmapper.put("uft8",       "UTF-8");
    encodingmapper.put("windows874", "x-windows-874");
    encodingmapper.put("xsjus",      "Shift_JIS");
    encodingmapper.put("xsjis",      "Shift_JIS");

  } 

  public static void doConvert(String In, String Out, String sample, Hashtable charsetHash) throws IOException {

      /* If a non warc.gz file is passed in, return right away.  We are prepared to handle warc.gz only! */
    if (!In.contains("warc.gz") )
      return;

    GZIPInputStream gzipInStream = new GZIPInputStream(new FileInputStream(In));
    // cast to a data input stream
    DataInputStream in = new DataInputStream(gzipInStream);
        
    GZIPOutputStream gzipOutStream = new GZIPOutputStream(new FileOutputStream(new File(Out)));
    PrintWriter barney = new PrintWriter(new OutputStreamWriter(gzipOutStream, "UTF-8"));
    String SampleDataDirectory = sample;
    boolean createSampleSet = false;
    if (SampleDataDirectory != "") {
      System.out.println("Creating a sample dataset: " + SampleDataDirectory);
      createSampleSet = true;
    }
    WarcRecord record;
    record = WarcRecord.readNextWarcRecord(in);
    barney.println(record);
 
    SortedMap sortmap = java.nio.charset.Charset.availableCharsets();
    Set charset = sortmap.keySet();
    Iterator mycharset;
    Iterator mycharset2;
    boolean foundCharset = false;
    int couldNotRecognizeCnt = 0;
    int couldNotFind = 0;
    int totalCnt = 0;
    String supportedEncoding = "";
    String trimSupportedEncoding = "";
    String htmlMetaEncode;
    String origEncoding = "";
    String encoding = "";
    String trimedEncoding = "";
    Boolean supported = false;
    int MAX_CNT = 20;

    LoadSpecialCaseEncoding(); 

   
    while ((record = WarcRecord.readNextWarcRecord(in)) != null) {
      totalCnt++;
      supported = false;
      foundCharset = false;

      String temp = new String(record.getContent(), "UTF-8");
      Matcher charsetMatch2 = charsetPattern.matcher(temp);
      if (charsetMatch2.find())  {
	origEncoding = new String(charsetMatch2.group(1));
	encoding = origEncoding.toLowerCase().replaceAll("[-_]", "");
	foundCharset = true;
      }    
      mycharset = charset.iterator();
      // find out if java supports/recognizes the encoding
      while (mycharset.hasNext() && (!supported)){
	supportedEncoding = (String) mycharset.next();
		
	if ( (supportedEncoding.contains("-")) && (supportedEncoding.contains("_")) ) {
	  //remove all the numbers
	  trimSupportedEncoding = supportedEncoding.replaceAll("[0-9]", "");
	}
	trimSupportedEncoding = supportedEncoding.replaceAll("[-_]", "");
		
	if (encoding.contains(trimSupportedEncoding.toLowerCase())) {
	  supported = true;
	}
      }

      // if we haven't found it look for these aliases
      if (!supported) {
        if (encodingmapper.containsKey(encoding) ) {
	  supportedEncoding = encodingmapper.get(encoding);
	  supported = true;
	}
      }
      /* if http header says "windows-1252 sniff to see what the html metadata says.
	 If it is different and valid, use the metadata encoding instead. 
      */
      if ((supportedEncoding.equalsIgnoreCase("windows-1252")) ||  (!supported)  ){
	byte[] contentInOctets = record.getContent();
	InputSource input = new InputSource(new ByteArrayInputStream(contentInOctets));
	String sniff = sniffCharacterEncoding (contentInOctets);
	// if we find something in the http metadata encoding, make sure it is a valid encoding and use it
	if (sniff != null) {
	  if (!sniff.equalsIgnoreCase(supportedEncoding)) {
	    //System.out.println("using html meta (" +  sniff +") instead of html header:(" + origEncoding + ").");
	    mycharset2 = charset.iterator();
	    supported = false;
	    // find out if java supports/recognizes the encoding
	    while (mycharset2.hasNext() && (!supported)){
	      htmlMetaEncode = (String) mycharset2.next();
	      if (sniff.equalsIgnoreCase(htmlMetaEncode)) {
		supported = true;
		//System.out.println("using: " + htmlMetaEncode);
		supportedEncoding = htmlMetaEncode;

	      }
	    }
	  }
	}
      }
      if (supported) {
		
	BufferedReader buf = new BufferedReader( new InputStreamReader(new ByteArrayInputStream(record.getContent()), supportedEncoding));
	StringBuffer outbuf = new StringBuffer();
	String line = "";
	while ((line = buf.readLine()) != null) {
	  foundCharset = false;	
	  Matcher metaMatch = metaPattern.matcher(line);
	  Matcher charsetMatch = charsetPattern.matcher(line);
	  if (charsetMatch.find())  {
	    origEncoding = new String(charsetMatch.group(1));
	    foundCharset = true;
	  }    
	  if (metaMatch.find()) {
	    foundCharset = true;
	  } 
			
	  if ( (foundCharset == true)) {
	    int s = 0;
	    int e = 0;
	    String replace = "UTF-8";
	    StringBuffer result = new StringBuffer();
	    while ((e=line.toLowerCase().indexOf(origEncoding.toLowerCase(),s)) >=0) {
	      result.append(line.substring(s,e));
	      result.append(replace);
	      s = e+origEncoding.length();
	    }
	    result.append(line.substring(s));
	    line = result.toString();
	  }            
	  outbuf.append(line);
	  outbuf.append("\n");
	}

	record.setContentLength(outbuf.toString().getBytes().length);
	barney.println(record.getHeaderString());
	barney.print(outbuf.toString());    

	// increment the counter
	Integer currentCount = (Integer)charsetHash.get(supportedEncoding);
	if (currentCount == null)
	  currentCount = 0;
	currentCount++;
	charsetHash.put(supportedEncoding, currentCount);
	// write out a sample converted record on the first of every charset and every so often.
	if ( ((currentCount % WRITEOUT_RECORD_EVERY == 0) || (currentCount == 1)) && (createSampleSet) ){

	  File file = new File (SampleDataDirectory);

	  if (file.mkdir()) {
	    System.out.println("directory created:" + SampleDataDirectory);
	  }
	  String path = SampleDataDirectory + "/" + supportedEncoding;
	  File file2 = new File (path);
	  if (file2.mkdir()) {
	    System.out.println("directory created:" + path);
	  }

	  String outfile = path + "/" + currentCount + ".html";
	  Writer out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(outfile), "UTF-8"));
	  out.write(outbuf.toString());
	  out.close();
	  System.out.println("charset: " + supportedEncoding + ", count: " + currentCount);

	}

	    	
      }
      else {
	if (foundCharset) {
	  //System.out.println("encoding not recognized: " + encoding);
	  encoding = "Unrecognized Charset: " + encoding;
	  couldNotRecognizeCnt++;
	}
	else {
	  encoding = "Charset not found";
	}
	System.out.println(record.getHeaderMetadataItem("WARC-TREC-ID") + ", " + record.getHeaderMetadataItem("WARC-Target-URI") + ", " + encoding);
	couldNotFind++;
      }
    }
        
    double tcnt = totalCnt;
    float percent = (float) ((couldNotFind/tcnt)*100.0);
    System.out.println("Total Records:" + totalCnt + "; Unrecoginzed: " + couldNotRecognizeCnt + 
		       "; 'charset='not found: " + couldNotFind + "; Records Skipped: " + percent + "%.");

    barney.close();
    in.close();
    gzipInStream.close();
    gzipOutStream.close();
  }
}
