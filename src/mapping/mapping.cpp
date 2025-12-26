
#include "mapping.h"

#include <windows.h>
#include <sstream>

#include "boostbiscuit.h"
#include "java/jvm/jvm_manager.h"


std::string get_mappings_path(const std::string& filePath) {
    char dllPath[MAX_PATH];

    GetModuleFileName(bb::h_module, dllPath, MAX_PATH);

    const std::string::size_type pos = std::string(dllPath).find_last_of("\\/");
    const std::string directory = std::string(dllPath).substr(0, pos);

    return directory + "\\" + filePath;
}


void parse_class_mapping(const std::string& line) {
    std::string lineContent = line.substr(4);

    const size_t delimiterPos = lineContent.find(' ');
    if (delimiterPos == std::string::npos)  return;

    const std::string readableName = lineContent.substr(0, delimiterPos);
    mapping::class_mappings.emplace(readableName, lineContent.substr(delimiterPos + 1));
}


void parse_field_mapping(const std::string& line) {
    std::string lineContent = line.substr(4);

    const size_t delimiterPos = lineContent.find(' ');
    if (delimiterPos == std::string::npos)  return;

    const size_t classDelimiterPos = lineContent.find('/', delimiterPos);
    if (classDelimiterPos == std::string::npos)  return;

    const std::string readableName = lineContent.substr(0, delimiterPos);
    mapping::field_mappings.emplace(readableName, lineContent.substr(classDelimiterPos + 1));
}


void parse_method_mapping(const std::string& line) {
    std::istringstream stream(line.substr(4));

    std::string readableClassMethod, readableSignature, obfClassMethod, obfSignature;
    stream >> readableClassMethod >> readableSignature >> obfClassMethod >> obfSignature;

    const std::string key = readableClassMethod + "/" + readableSignature;

    const size_t methodDelimiterPos = obfClassMethod.rfind('/');
    if (methodDelimiterPos == std::string::npos)  return;

    mapping::method_mappings.emplace(key, std::make_pair(obfClassMethod.substr(methodDelimiterPos + 1), obfSignature));
}


bool load_mappings(const std::string& srg_fp) {
    std::ifstream file(srg_fp);

    if (!file.is_open()) {
        logger::get().error("Failed to open mappings file: " + srg_fp);
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.find("CL:") == 0)  parse_class_mapping(line);
        else if (line.find("FD:") == 0)  parse_field_mapping(line);
        else if (line.find("MD:") == 0)  parse_method_mapping(line);
    }

    return true;
}


bool mapping::initialize() {
    const ClientBrand cl_brand = jvm_manager::get().get_client_brand();

    std::string srg_file_name;
    switch (cl_brand) {
        case ClientBrand::LUNAR:
            srg_file_name = "res\\vanilla189.srg";
            break;
        case ClientBrand::BADLION:
            srg_file_name = "res\\vanilla189.srg";
            break;
        case ClientBrand::VANILLA:
            srg_file_name = "res\\vanilla189.srg";
            break;
        case ClientBrand::UNSUPPORTED:
            srg_file_name = "wtf";
            break;
        default:
            break;
    }

    if (srg_file_name == "wtf")  // not possible
        return false;

    const std::string srg_fp = get_mappings_path(srg_file_name);

    return load_mappings(srg_fp);
}


std::string& mapping::get_class_path(const std::string& class_path) {
    const auto it = class_mappings.find(class_path);
    if (it != class_mappings.end())
        return it->second;

    logger::get().warn("Class not found! (key: " + class_path + ")");
    static std::string empty_string;
    return empty_string;
}


std::pair<std::string, std::string>& mapping::get_method_name_and_signature(const std::string& method_name, const std::string& method_signature) {
    const std::string key = method_name + "/" + method_signature;

    const auto it = method_mappings.find(key);
    if (it != method_mappings.end())
        return it->second;

    logger::get().warn("Method not found! (key: " + key + ")");
    static std::pair<std::string, std::string> empty_pair;
    return empty_pair;
}


std::string& mapping::get_field_name(const std::string& field_name) {
    const auto it = field_mappings.find(field_name);
    if (it != field_mappings.end())
        return it->second;

    logger::get().error("Field not found! (key: " + field_name + ")");
    static std::string empty_string;
    return empty_string;
}
