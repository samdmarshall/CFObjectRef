import sys
import json
import os
import string
from StringIO import StringIO

gen_dir = '';

def file_exists(path):
    return os.path.exists(path)


def make_dir(path):
    if file_exists(path) == False:
        os.mkdir(path)


def make_sym(original, path):
    if file_exists(path) == False:
        os.symlink(original, path)


def get_file_name(path):
    name, extension = os.path.splitext(os.path.basename(path));
    return name;

def CreateBoilerPlateStart(name, exten):
    file_name = name + "." + exten.lower();
    file_path = os.path.join(gen_dir, file_name);
    fd = open(file_path, "w");
    print >> fd, "// This is an auto-generated file";
    print >> fd, "#ifndef __" + name.upper() + "_" + exten.upper() + "_";
    print >> fd, "#define __" + name.upper() + "_" + exten.upper() + "_";
    print >> fd, "";
    return fd;


def ComposeName(prefix, name, affix):
    return prefix + name + affix;


def __CFRef(name):
    return ComposeName("",name,"Ref");


def CreateIvarName(prop_name):
    func = lambda s: s[:1].lower() + s[1:] if s else ''
    ivar_name = "_" + func(prop_name);
    return ivar_name;


def CFObjectCreateForClass(fd, name, args):
    print >> fd, "" + __CFRef(name) + " " + ComposeName("",name,"Create") + "(CFAllocatorRef allocator);";
    print >> fd, "";


def CreateCFObjectWithNameAndProperties(fd, name, ivars):
    print >> fd, "#include <CoreFoundation/CoreFoundation.h>";
    print >> fd, "#include \"CFRuntime.h\"";
    print >> fd, "";
    print >> fd, "struct " + ComposeName("__",name,"Ivars") + " {";
    for ivar in ivars:
        print >> fd, "  " + ivars[ivar] + " " + CreateIvarName(ivar) + ";"; 
    print >> fd, "};";
    print >> fd, "struct " + ComposeName("__",name,"") + " {";
    print >> fd, "  CFRuntimeBase _base;";
    print >> fd, "  struct " + ComposeName("__",name,"Ivars") + " ivars;";
    print >> fd, "};";
    print >> fd, "typedef const struct " + ComposeName("__",name,"") + " * " + __CFRef(name) + ";";
    print >> fd, "CFTypeID " + ComposeName("",name,"GetTypeID") + "(void);";
    print >> fd, "";


def CFObjectCreateGetSetForIvar(fd, name, ivars):
    for ivar in ivars:
        print >> fd, ivars[ivar] + " " + ComposeName(name,"Get",ivar) + "(" + __CFRef(name) + " ref);";
        print >> fd, "void " + ComposeName(name,"Set",ivar) + "(" + __CFRef(name) + " ref, " + ivars[ivar] + " value);"
        print >> fd, "";


def CreateHeader(name, ivars):
    fd = CreateBoilerPlateStart(name, "h");
    CreateCFObjectWithNameAndProperties(fd, name, ivars);
    CFObjectCreateForClass(fd, name, ivars);
    CFObjectCreateGetSetForIvar(fd, name, ivars);
    print >> fd, "";
    print >> fd, "#endif";
    fd.close();


def CFObjectClassID(name):
    return ComposeName("_k",name,"ID");


def CFObjectClassRuntimeDefinition(name):
    return ComposeName("_k",name,"Class");


def CFObjectClassInitializationFunc(name):
    return ComposeName("__",name,"ClassInitialize")


def CFObjectInitializeClass(fd, name):
    print >> fd, "static CFTypeID " + CFObjectClassID(name) + " = _kCFRuntimeNotATypeID;";
    print >> fd, "static CFRuntimeClass " + CFObjectClassRuntimeDefinition(name) + " = {0};";
    print >> fd, "void " + CFObjectClassInitializationFunc(name) + "(void) {";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".version = 0;";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".className = \"" + name + "\";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".init = " + ComposeName("__",name,"Init") + ";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".copy = " + ComposeName("__",name,"Copy") +";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".finalize = " + ComposeName("__",name,"Finalize") + ";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".equal = " + ComposeName("__",name,"Equal") + ";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".hash = " + ComposeName("__",name,"Hash") + ";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".copyFormattingDesc = " + ComposeName("__",name,"CopyFormattingDesc") + ";";
    print >> fd, "	" + CFObjectClassRuntimeDefinition(name) + ".copyDebugDesc = " + ComposeName("__",name,"CopyDebugDesc") + ";";
    print >> fd, "	" + CFObjectClassID(name) + " = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&" + CFObjectClassRuntimeDefinition(name) + ");";
    print >> fd, "}";
    print >> fd, "";
    print >> fd, "CFTypeID " + ComposeName("",name,"GetTypeID") + "(void) {";
    print >> fd, "    return " + CFObjectClassID(name) + ";";
    print >> fd, "}";
    print >> fd, "";


def CFObjectCreate(fd, name):
    print >> fd, "  if (" + CFObjectClassID(name) + " == _kCFRuntimeNotATypeID) {";
    print >> fd, "    " + CFObjectClassInitializationFunc(name) + "();";
    print >> fd, "  }"
    print >> fd, "  struct " + ComposeName("__",name,"") + " *obj = NULL;";
    print >> fd, "  CFIndex extra = sizeof(struct " + ComposeName("__",name,"") + ") - sizeof(CFRuntimeBase);";
    print >> fd, "  obj = (struct " + ComposeName("__",name,"") + " *)_CFRuntimeCreateInstance(allocator, " + CFObjectClassID(name) + ", extra, NULL);";

def CFObjectCreateImpForCreate(fd, name):
    print >> fd, __CFRef(name) + " " + ComposeName("",name,"Create") + "(CFAllocatorRef allocator) {";
    CFObjectCreate(fd, name);
    print >> fd, "";
    print >> fd, "  return (" + __CFRef(name) + ")obj;";
    print >> fd, "}";

def CFObjectCreateGetterNameForIvar(name, ivar):
    return ComposeName(name,"Get",ivar);


def CFObjectCreateImpForGetIvar(name, ivar_type, ivar):
    return "" + ivar_type + " " + CFObjectCreateGetterNameForIvar(name, ivar) + "(" + __CFRef(name)+ " cf)";

 
def CFObjectSynthesizeImpForGetIvar(fd, name, ivar, ivars):
    print >> fd, "" + CFObjectCreateImpForGetIvar(name, ivars[ivar], ivar) + " {";
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  return objref->ivars." + CreateIvarName(ivar) + ";";
    print >> fd, "}";


def CFObjectCreateSetterNameForIvar(name, ivar):
    return ComposeName(name,"Set",ivar);


def CFObjectCreateImpForSetIvar(fd, name, ivar, ivars):
    print >> fd, "void " + CFObjectCreateSetterNameForIvar(name, ivar)+ "(" + __CFRef(name)+" cf, " + ivars[ivar] + " value) {";
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  #warning Missing logic for " + __CFRef(name) + " Setter \"" + ivar + "\"."; 
    print >> fd, "}"


def __CFObjectBaseDeclaration(fd, name, call_type, return_type, args):
     print >> fd, "static " + return_type + " " + ComposeName("__",name,call_type) + args + "{";


def CFObjectInit(fd, name):
    __CFObjectBaseDeclaration(fd, name, "Init", "void", "(CFTypeRef cf)");
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "}";
    print >> fd, "";

def CFObjectCopy(fd, name):
    __CFObjectBaseDeclaration(fd, name, "Copy", "CFTypeRef", "(CFAllocatorRef allocator, CFTypeRef cf)");
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  " + __CFRef(name) + " newref = " + ComposeName("",name,"Create") + "(kCFAllocatorDefault);";
    print >> fd, "  memcpy(newref->ivars, objref->ivars, sizeof(struct " + ComposeName("__",name,"Ivars") + "));";
    print >> fd, "  return newref;";
    print >> fd, "}";
    print >> fd, "";

def CFObjectRelease(fd, name):
    __CFObjectBaseDeclaration(fd, name, "Finalize", "void", "(CFTypeRef cf)");
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  #warning Missing logic for " + __CFRef(name) + " CFRelease()."; 
    print >> fd, "}";
    print >> fd, "";

def CFObjectEqual(fd, name):
    __CFObjectBaseDeclaration(fd, name, "Equal", "Boolean", "(CFTypeRef cf1, CFTypeRef cf2)");
    print >> fd, "  " + __CFRef(name) + " objref1 = (" + __CFRef(name) + ")cf1;";
    print >> fd, "  " + __CFRef(name) + " objref2 = (" + __CFRef(name) + ")cf2;";
    print >> fd, "  return (objref1 == objref2);";
    print >> fd, "}";
    print >> fd, "";

def CFObjectHash(fd, name):
    __CFObjectBaseDeclaration(fd, name, "Hash", "CFHashCode", "(CFTypeRef cf)");
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  return (CFHashCode)CFGetAllocator(objref);";
    print >> fd, "}";
    print >> fd, "";

def CFObjectCopyDescription(fd, name):
    __CFObjectBaseDeclaration(fd, name, "CopyFormattingDesc", "CFStringRef", "(CFTypeRef cf, CFDictionaryRef formatOpts)");
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  return CFStringCreateWithFormat(CFGetAllocator(objref), formatOpts, CFSTR(\"<" + __CFRef(name) + " %p>\"), CFGetAllocator(objref));"
    print >> fd, "}";
    print >> fd, "";

def CFObjectCopyDebugDescription(fd, name):
    __CFObjectBaseDeclaration(fd, name, "CopyDebugDesc", "CFStringRef", "(CFTypeRef cf)");
    print >> fd, "  " + __CFRef(name) + " objref = (" + __CFRef(name) + ")cf;";
    print >> fd, "  return CFStringCreateWithFormat(CFGetAllocator(objref), formatOpts, CFSTR(\"<" + __CFRef(name) + " %p>\"), CFGetAllocator(objref));"
    print >> fd, "}";
    print >> fd, "";

def CFObjectCreateInternals(fd, name, ivars):
    print >> fd, "#include \"" + name + ".h\"";
    print >> fd, "";
    print >> fd, "#pragma mark -";
    print >> fd, "#pragma mark Private";
    print >> fd, "";
    CFObjectInit(fd, name);
    CFObjectCopy(fd, name);
    CFObjectRelease(fd, name);
    CFObjectEqual(fd, name);
    CFObjectHash(fd, name);
    CFObjectCopyDescription(fd, name);
    CFObjectCopyDebugDescription(fd, name);
    CFObjectInitializeClass(fd, name);
    print >> fd, "";
    print >> fd, "#pragma mark -";
    print >> fd, "#pragma mark Public";
    print >> fd, "";
    CFObjectCreateImpForCreate(fd, name);
    for ivar in ivars:
        print >> fd, "";
        CFObjectSynthesizeImpForGetIvar(fd, name, ivar, ivars);
        print >> fd, "";
        CFObjectCreateImpForSetIvar(fd, name, ivar, ivars);
        print >> fd, "";


def CreateImplementation(name, ivars):
    fd = CreateBoilerPlateStart(name, "c");
    CFObjectCreateInternals(fd, name, ivars);
    print >> fd, "";
    print >> fd, "#endif";
    fd.close();


if len(sys.argv) > 2:
    gen_dir = os.path.join(os.path.basename(sys.argv[1]), 'GeneratedObjects');
    make_dir(gen_dir);
    for file_path in sys.argv[2:]:
        with open(file_path, 'r') as template:
            object_template = json.load(StringIO(template.read()));
            name = object_template['Name'];
            ivars = object_template['Ivars'];
            CreateHeader(name, ivars);
            CreateImplementation(name, ivars);
else:
    print 'Please supply cf-object templates to generate files!'