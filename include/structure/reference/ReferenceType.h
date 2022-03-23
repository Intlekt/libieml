#pragma once


namespace ieml::structure {

class ReferenceType {
public:
    /* A Reference type is the accepted type object for a given argument. It can be : 
     *      1) a dtype (only utf8 string at the moment) 
     *      2) a path tree            (~type)
     *      3) a path tree instance   (~object)
     */

};

class ReferenceDType : public ReferenceType {

};

class PathTreeReferenceType : public ReferenceType {

};

class InstanceReferenceType : public ReferenceType {

};

}