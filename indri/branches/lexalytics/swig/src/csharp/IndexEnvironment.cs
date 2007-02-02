/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.29
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Indri {

using System;
using System.Runtime.InteropServices;

public class IndexEnvironment : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal IndexEnvironment(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(IndexEnvironment obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~IndexEnvironment() {
    Dispose();
  }

  public virtual void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      indri_csharpPINVOKE.delete_IndexEnvironment(swigCPtr);
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
  }

  public IndexEnvironment() : this(indri_csharpPINVOKE.new_IndexEnvironment(), true) {
  }

  public void setDocumentRoot(string documentRoot) {
    indri_csharpPINVOKE.IndexEnvironment_setDocumentRoot(swigCPtr, documentRoot);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setAnchorTextPath(string anchorTextRoot) {
    indri_csharpPINVOKE.IndexEnvironment_setAnchorTextPath(swigCPtr, anchorTextRoot);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setOffsetMetadataPath(string offsetMetadataRoot) {
    indri_csharpPINVOKE.IndexEnvironment_setOffsetMetadataPath(swigCPtr, offsetMetadataRoot);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setOffsetAnnotationsPath(string offsetAnnotationsRoot) {
    indri_csharpPINVOKE.IndexEnvironment_setOffsetAnnotationsPath(swigCPtr, offsetAnnotationsRoot);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void addFileClass(string name, string iterator, string parser, string tokenizer, string startDocTag, string endDocTag, string endMetadataTag, StringVector include, StringVector exclude, StringVector index, StringVector metadata, ConfMap conflations) {
    indri_csharpPINVOKE.IndexEnvironment_addFileClass__SWIG_0(swigCPtr, name, iterator, parser, tokenizer, startDocTag, endDocTag, endMetadataTag, StringVector.getCPtr(include), StringVector.getCPtr(exclude), StringVector.getCPtr(index), StringVector.getCPtr(metadata), ConfMap.getCPtr(conflations));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public Specification getFileClassSpec(string name) {
    IntPtr cPtr = indri_csharpPINVOKE.IndexEnvironment_getFileClassSpec(swigCPtr, name);
    Specification ret = (cPtr == IntPtr.Zero) ? null : new Specification(cPtr, false);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void addFileClass(Specification spec) {
    indri_csharpPINVOKE.IndexEnvironment_addFileClass__SWIG_1(swigCPtr, Specification.getCPtr(spec));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void deleteDocument(int documentID) {
    indri_csharpPINVOKE.IndexEnvironment_deleteDocument(swigCPtr, documentID);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setIndexedFields(StringVector fieldNames) {
    indri_csharpPINVOKE.IndexEnvironment_setIndexedFields(swigCPtr, StringVector.getCPtr(fieldNames));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setNumericField(string fieldName, bool isNumeric, string parserName) {
    indri_csharpPINVOKE.IndexEnvironment_setNumericField__SWIG_0(swigCPtr, fieldName, isNumeric, parserName);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setNumericField(string fieldName, bool isNumeric) {
    indri_csharpPINVOKE.IndexEnvironment_setNumericField__SWIG_1(swigCPtr, fieldName, isNumeric);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setMetadataIndexedFields(StringVector forward, StringVector backward) {
    indri_csharpPINVOKE.IndexEnvironment_setMetadataIndexedFields(swigCPtr, StringVector.getCPtr(forward), StringVector.getCPtr(backward));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setStopwords(StringVector stopwords) {
    indri_csharpPINVOKE.IndexEnvironment_setStopwords(swigCPtr, StringVector.getCPtr(stopwords));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setStemmer(string stemmer) {
    indri_csharpPINVOKE.IndexEnvironment_setStemmer(swigCPtr, stemmer);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setMemory(long memory) {
    indri_csharpPINVOKE.IndexEnvironment_setMemory(swigCPtr, memory);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void setNormalization(bool normalize) {
    indri_csharpPINVOKE.IndexEnvironment_setNormalization(swigCPtr, normalize);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void create(string repositoryPath, IndexStatus callback) {
    indri_csharpPINVOKE.IndexEnvironment_create__SWIG_0(swigCPtr, repositoryPath, IndexStatus.getCPtr(callback));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void create(string repositoryPath) {
    indri_csharpPINVOKE.IndexEnvironment_create__SWIG_1(swigCPtr, repositoryPath);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void open(string repositoryPath, IndexStatus callback) {
    indri_csharpPINVOKE.IndexEnvironment_open__SWIG_0(swigCPtr, repositoryPath, IndexStatus.getCPtr(callback));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void open(string repositoryPath) {
    indri_csharpPINVOKE.IndexEnvironment_open__SWIG_1(swigCPtr, repositoryPath);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void close() {
    indri_csharpPINVOKE.IndexEnvironment_close(swigCPtr);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void addFile(string fileName) {
    indri_csharpPINVOKE.IndexEnvironment_addFile__SWIG_0(swigCPtr, fileName);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public void addFile(string fileName, string fileClass) {
    indri_csharpPINVOKE.IndexEnvironment_addFile__SWIG_1(swigCPtr, fileName, fileClass);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
  }

  public int addString(string fileName, string fileClass, MetadataPairSTDVector metadata) {
    int ret = indri_csharpPINVOKE.IndexEnvironment_addString(swigCPtr, fileName, fileClass, MetadataPairSTDVector.getCPtr(metadata));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int addParsedDocument(ParsedDocument document) {
    int ret = indri_csharpPINVOKE.IndexEnvironment_addParsedDocument(swigCPtr, ParsedDocument.getCPtr(document));
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int documentsIndexed() {
    int ret = indri_csharpPINVOKE.IndexEnvironment_documentsIndexed(swigCPtr);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int documentsSeen() {
    int ret = indri_csharpPINVOKE.IndexEnvironment_documentsSeen(swigCPtr);
    if (indri_csharpPINVOKE.SWIGPendingException.Pending) throw indri_csharpPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}