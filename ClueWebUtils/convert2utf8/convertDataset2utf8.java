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
import java.io.File;
import java.io.IOException;
import java.lang.Object;



/*
  This code will traverse the corpus, pick pass the files into the warc doConvert
  routine from ConverWarc.java for the conversion.  An optional sample can be created
  as well.  The sample is individual records saved to an html file.
*/

public class convertDataset2utf8 {

    public static void main(String[] args)throws IOException{
	String samplePath;
	if ((args.length != 2) && (args.length !=2 )) {
	    System.out.println("usage:");
	    System.out.println("convertDataset2utf8 pathToCorpus pathToOutput [pathToSampleSet]");
	    System.out.println("");
	    System.out.println("");
	    return;

	}
	if (args.length == 2) {
	    samplePath = "";
	}
	else
	    samplePath = args[2];

	SortedMap sortmap = java.nio.charset.Charset.availableCharsets();
	Set charset = sortmap.keySet();
	Iterator mycharset;
	Hashtable cSetHash = new Hashtable();
	String jCharsetId = "";
	
	mycharset = charset.iterator();
	
	// build hash table
	while (mycharset.hasNext() ) {
	    jCharsetId = (String) mycharset.next();
	    cSetHash.put(jCharsetId, new Integer(0));
	}
	ConvertWarc cWarc = new ConvertWarc();

	showDir(args[0], args[1], new File(args[0]), cWarc, cSetHash, samplePath, true);
    }
    
    static void showDir(String d, String outPath, File file, ConvertWarc warcd, Hashtable hash, String SamplePath, Boolean first) throws IOException {
	//String BasePath = d;
	String CurrentPath;
	String CurrentOutPath;

	if (first) {
	    CurrentPath = d;
	}
	else {
	    CurrentPath = d +'/'+ file.getName();
	}
	CurrentOutPath = outPath + '/' + file.getName();

	if (file.isDirectory()) {
	  

	    File F = new File(CurrentOutPath);
	    if (F.mkdirs()) {
	    	System.out.println("created directory: " + CurrentOutPath);
	    }

	    File[] files = file.listFiles();
	    for (int i = 0; i < files.length; i++) {
		if (!first) {
		    CurrentPath = d +'/'+ file.getName();
		}
		showDir(CurrentPath, CurrentOutPath, files[i], warcd, hash, SamplePath, false);
	    }
	}
	else {
	    System.out.println("doConvert " + CurrentPath + " " + CurrentOutPath + " " + SamplePath);
	    warcd.doConvert(CurrentPath, CurrentOutPath, SamplePath, hash);
	}
    }
}
