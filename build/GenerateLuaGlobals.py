import sys
quiet = "-quiet" in sys.argv

def log(_):
    if not quiet:
        print(_)
try:
    from lxml import etree
    log("running with lxml.etree")
except ImportError:
    import xml.etree.ElementTree as etree
    log("running with Python's xml.etree.ElementTree")

indexTree = etree.parse('docs/xml/index.xml')

indexRoot = indexTree.getroot()

Types = {
    "int": "number",
    "float": "number",
    "const char *": "string",
    "sol::variadic_args": "...",
}

ReturnTypeText = {
    "number": "0",
    "string": "\"\"",
}

def parse_tree(tree):
    for child in tree:
        kind = child.get('kind')
        # log(kind)
        match kind:
            case 'namespace':
                parse_namespace(child)

def parse_namespace(tree):
    namespace = tree.find('name').text
    refid = tree.get('refid')
    log('[namespace] '+ namespace)

    f.write(f'---@class {namespace}\n')
    f.write(f'{namespace} = {{}}\n\n')

    namespaceTree = etree.parse('docs/xml/'+refid+'.xml').getroot()
    for child in namespaceTree.find(".//*[@kind='func']"):
        parse_namespace_function(namespace, child)

def parse_namespace_function(namespace, tree):
    name = tree.find('name').text
    log('\t[func] ' + name)
    descr = tree.find('briefdescription').find('para').text

    f.write(f'--- {descr}\n')

    params = tree.findall('param')
    paramtext = ""
    for param in params:
        cpptype = param.find('type').text
        paramname = param.find('declname').text
        type = Types[cpptype]

        if type == "...":
            paramname = "..."

        if paramtext == "":
            paramtext += paramname
        else:
            paramtext += f", {paramname}"
            
        f.write(f'---@param {paramname} {type}\n')

    returncpp = tree.find('type').text
    returntext = ""
    if returncpp != "void":
        returntype = Types[returncpp]
        f.write(f'---@return {returntype}\n')
        returntext = ReturnTypeText[returntype]
    
    if returntext != "":
        f.write(f'function {namespace}.{name}({paramtext}) return {returntext} end\n\n')
    else:
        f.write(f'function {namespace}.{name}({paramtext}) end\n\n')
    

with open("scripts/Meta/globals.lua", mode="wt") as f:
    parse_tree(indexRoot)
