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

bool get_inode_metadata(LevelDBAdaptor* adaptor, size_t inode_id, inode_metadata_t* &inode_metadata) {
    std::string metadata_key = decode_inode_metadata(inode_id);
    std::string metadata_value;
    if (!adaptor->GetValue(metadata_key, metadata_value)) {
        inode_metadata = nullptr;
        spdlog::error("get inode metadata inode_id - {}: inode metadata doesn't exist", inode_id);
        exit(1);
    }
    inode_metadata = new inode_metadata_t;
    const inode_metadata_t* metadata = reinterpret_cast<const inode_metadata_t *>(metadata_value.data());
    std::memcpy(&inode_metadata, metadata, sizeof(inode_metadata_t));
    if (ENABELD_LOG) {
        spdlog::info("get inode metadata: {}", inode_metadata->debug());
    }
    return true;
}

bool put_inode_metadata(LevelDBAdaptor* adaptor, size_t inode_id, inode_metadata_t* &inode_metadata) {
    if (inode_metadata == nullptr) {
        spdlog::error("put inode metadata: inode metadata doesn't exist");
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("put inode metadata: {}", inode_metadata->debug());
    }
    std::string metadata_key = decode_inode_metadata(inode_id);
    std::string metadata_value = std::string(reinterpret_cast<const char*>(&inode_metadata), sizeof(inode_metadata_t));
    if (!adaptor->Insert(metadata_key, metadata_value)) {
        delete inode_metadata;
        spdlog::error("put inode metadata inode_id - {}: kv store interanl error", inode_id);
        exit(1);
    }
    delete inode_metadata;
    return true;
}

bool get_inode_data(LevelDBAdaptor* adaptor, size_t inode_id, inode_data_t* &inode_data) {
    std::string data_key = decode_inode_data(inode_id);
    std::string data_value;
    if (!adaptor->GetValue(data_key, data_value)) {
        inode_data = nullptr;
        spdlog::error("get inode data inode_id - {}: inode data doesn't exist", inode_id);
        exit(1);
    }
    inode_data = new inode_data_t;
    try {
        nlohmann::json json_map = nlohmann::json::parse(data_value);
        inode_data->map = json_map.get<std::unordered_map<std::string, std::string>>();
    } catch (const std::exception& e) {
        inode_data = nullptr;
        spdlog::error("get inode data inode_id -{}: unresolved data format", inode_id);
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("get inode metadata: {}", inode_data->debug());
    }
    return true;
}

bool put_inode_data(LevelDBAdaptor* adaptor, size_t inode_id, inode_data_t* &inode_data) {
    if (inode_data == nullptr) {
        spdlog::error("put inode data: inode data doesn't exist");
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("put inode data: {}", inode_data->debug());
    }
    std::string data_key = decode_inode_data(inode_id);
    nlohmann::json json_map = inode_data->map;
    std::string data_value = json_map.dump();
    if (!adaptor->Insert(data_key, data_value)) {
        delete inode_data;
        spdlog::error("put inode data inode_id - {}: kv store interanl error", inode_id);
        exit(1);
    }
    delete inode_data;
    return true;
}

bool get_inode(LevelDBAdaptor* adaptor, size_t inode_id, inode_t* &inode) {
    inode_metadata_t* metadata;
    inode_data_t* data;
    if (!get_inode_metadata(adaptor, inode_id, metadata)) {
        inode = nullptr;
        return false;
    }
    if (!get_inode_data(adaptor, inode_id, data)) {
        delete metadata;
        inode = nullptr;
        return false;
    }
    inode = new inode_t;
    std::memcpy(&inode->metadata, metadata, sizeof(inode_metadata_t));
    inode->data->map = data->map;
    delete metadata;
    delete data;
    if (ENABELD_LOG) {
        spdlog::info("get inode: {}", inode->debug());
    }
    return true;
}

bool put_inode(LevelDBAdaptor* adaptor, size_t inode_id, inode_t* inode) {
    if (inode == nullptr) {
        spdlog::error("put inode: inode doesn't exist");
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("put inode: {}", inode->debug());
    }
    if (!put_inode_metadata(adaptor, inode_id, inode->metadata)) {
        delete inode;
        return false;
    }
    if (!put_inode_data(adaptor, inode_id, inode->data)) {
        delete inode;
        return false;
    }
    delete inode;
    return true;
}

}