BEGIN {
  print "//";
  print "// Warning: This file is automatically generated.";
  print "//";

  print "#include \"indri/Unpacker.hpp\"";
  print "#include \"indri/QuerySpec.hpp\"";
  print "#include \"indri/XMLNode.hpp\"";
  print "#include \"lemur/Exception.hpp\"";
  print "";

  print    "indri::lang::Node* indri::lang::Unpacker::_unpack( indri::xml::XMLNode* child ) {";
  print    "  Node* result = 0;";
  print    "  std::string type = child->getAttribute( \"type\" );";
  print    "  std::string name = child->getAttribute( \"name\" );";
  print    "";

  count = 0;
}

$1 == "class" {
  if( count == 0 ) {
    printf "  if( type == \"%s\" ) {\n", $2;
  } else {
    printf "  } else if( type == \"%s\" ) {\n", $2;
  }

  count++;

  printf   "    result = new %s(*this);\n", $2;
}

END {
  print    "  } else {";
  print    "    LEMUR_THROW( LEMUR_RUNTIME_ERROR, \"Unknown node type '\" + type + \"' found during unpacking.\" );";
  print    "  }";
  print    "";
  print    "  result->setNodeName( name );";
  print    "  return result;";
  print    "}";
  print    "";
}

