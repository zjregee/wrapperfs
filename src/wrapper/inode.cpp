#include "wrapper/inode.h"

namespace wrapperfs {

std::string decode_inode_metadata(size_t inode_id) {
    std::ostringstream oss;
    oss << "inode_metadata:";
    oss << std::setw(6) << std::setfill('0') << inode_id;
    return oss.str();
}

std::string decode_inode_data(size_t inode_id) {
    std::ostringstream oss;
    oss << "inode_data:";
    oss << std::setw(6) << std::setfill('0') << inode_id;
    return oss.str();
}

bool get_inode(LevelDBAdaptor* adaptor, size_t inode_id, inode_t* &inode) {
    if (ENABELD_LOG) {
        spdlog::info("get_inode: inode_id - {}", inode_id);
    }
    std::string metadata_key = decode_inode_metadata(inode_id);
    std::string data_key = decode_inode_data(inode_id);
    std::string metadata_value;
    std::string data_value;
    if (!adaptor->GetValue(metadata_key, metadata_value)) {
        inode = nullptr;
        spdlog::error("get inode error");
        return false;
    }
    if (!adaptor->GetValue(data_key, data_value)) {
        inode = nullptr;
        spdlog::error("get inode error");
        return false;
    }
    inode = new inode_t;
    const inode_metadata* metadata = reinterpret_cast<const inode_metadata *>(metadata_value.data());
    std::memcpy(&(inode->metadata), metadata, sizeof(inode_metadata));
    try {
        nlohmann::json json_map = nlohmann::json::parse(data_value);
        inode->data.map = json_map.get<std::unordered_map<std::string, std::string>>();
    } catch (const std::exception& e) {
        spdlog::error("get inode error");
        inode = nullptr;
        return false;
    }
    return true;
}

bool put_inode(LevelDBAdaptor* adaptor, size_t inode_id, inode_t* inode) {
    if (ENABELD_LOG) {
        spdlog::info("put_inode: inode_id - {}", inode_id);
    }
    if (inode == nullptr) {
        spdlog::error("put inode error");
        return false;
    }
    std::string metadata_key = decode_inode_metadata(inode_id);
    std::string data_key = decode_inode_data(inode_id);
    std::string metadata_value;
    std::string data_value;
    metadata_value = std::string(reinterpret_cast<const char*>(&(inode->metadata)), sizeof(inode_metadata));
    nlohmann::json json_map = inode->data.map;
    data_value = json_map.dump();
    if (!adaptor->Insert(metadata_key, metadata_value)) {
        spdlog::error("put inode error");
        delete inode;
        return false;
    }
    if (!adaptor->Insert(data_key, data_value)) {
        spdlog::error("put inode error");
        delete inode;
        return false;
    }
    delete inode;
    return true;
}
    
}