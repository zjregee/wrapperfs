#include "wrapper/wrapper.h"

namespace wrapperfs {

std::string decode_entries(entries_t* entries) {
    std::ostringstream oss;
    oss << "entries:";
    switch (entries->tag) {
        case directory_relation:
            oss << "d:";
            break;
    }
    oss << std::setw(6) << std::setfill('0') << entries->wrapper_id;
    return oss.str();
}

std::string decode_relation(relation_t* relation) {
    std::ostringstream oss;
    oss << "relations:";
    switch (relation->tag) {
        case directory_relation:
            oss << "d:";
            break;
    }
    oss << std::setw(6) << std::setfill('0') << relation->wrapper_id << ":";
    oss << relation->distance;
    return oss.str();
}

std::string decode_location(location_t* location) {
    std::ostringstream oss;
    oss << "location:";
    switch (location->tag) {
        case directory_relation:
            oss << "d:";
            break;
    }
    oss << std::setw(6) << std::setfill('0') << location->wrapper_id << ":";
    return oss.str();
}

bool get_entries(LevelDBAdaptor* adaptor, entries_t* &entries) {
    std::string key = decode_entries(entries);
    std::string value;
    if (!adaptor->GetValue(key, value)) {
        entries = nullptr;
        spdlog::warn("get entries tag - {} wrapper_id - {}: entries doesn't exist", entries->tag, entries->wrapper_id);
        return false;
    }
    entries = new entries_t;
    try {
        nlohmann::json json = nlohmann::json::parse(value);
        json.at("tag").get_to(entries->tag);
        json.at("wrapper_id").get_to(entries->wrapper_id);
        json.at("list").get_to(entries->list);
    } catch (const std::exception& e) {
        entries = nullptr;
        spdlog::error("get entries tag - {} wrapper_id - {}: unresolved data format", entries->tag, entries->wrapper_id);
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("get entries: {}", entries->debug());
    }
    return true;
}

bool put_entries(LevelDBAdaptor* adaptor, entries_t* entries) {
    if (entries == nullptr) {
        spdlog::error("put entries: entries doesn't exist");
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("put entries: {}", entries->debug());
    }
    std::string key = decode_entries(entries);
    nlohmann::json json = nlohmann::json{{"tag", entries->tag},
                                          {"wrapper_id", entries->wrapper_id},
                                          {"list", entries->list}};
    std::string value = json.dump();
    if (!adaptor->Insert(key, value)) {
        delete entries;
        spdlog::error("put entries tag - {} wrapper_id - {}: kv store interanl error", entries->tag, entries->wrapper_id);
        exit(1);
    }
    delete entries;
    return true;
}

bool get_relation(LevelDBAdaptor* adaptor, relation_t* &relation) {
    std::string key = decode_relation(relation);
    std::string value;
    if (!adaptor->GetValue(key, value)) {
        relation = nullptr;
        spdlog::warn("get relation tag - {} wrapper_id - {} distance - {}: relation doesn't exist", relation->tag, relation->wrapper_id, relation->distance);
        return false;
    }
    relation = new relation_t;
    try {
        nlohmann::json json = nlohmann::json::parse(value);
        json.at("tag").get_to(relation->tag);
        json.at("wrapper_id").get_to(relation->wrapper_id);
        json.at("distance").get_to(relation->distance);
        json.at("next_wrapper_id").get_to(relation->next_wrapper_id);
    } catch (const std::exception& e) {
        relation = nullptr;
        spdlog::error("get relation tag - {} wrapper_id - {} distance - {}: unresolved data format", relation->tag, relation->wrapper_id, relation->distance);
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("get relation: {}", relation->debug());
    }
    return true;
}

bool put_relation(LevelDBAdaptor* adaptor, relation_t* relation) {
    if (relation == nullptr) {
        spdlog::error("put relation: relation doesn't exist");
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("put relation: {}", relation->debug());
    }
    std::string key = decode_relation(relation);
    nlohmann::json json = nlohmann::json{{"tag", relation->tag},
                                          {"wrapper_id", relation->wrapper_id},
                                          {"distance", relation->distance},
                                          {"next_wrapper_id", relation->next_wrapper_id}};
    std::string value = json.dump();
    if (!adaptor->Insert(key, value)) {
        delete relation;
        spdlog::error("put relation tag - {} wrapper_id - {} distance - {}: kv store interanl error", relation->tag, relation->wrapper_id, relation->distance);
        exit(1);
    }
    delete relation;
    return true;
}

bool get_location(LevelDBAdaptor* adaptor, location_t* &location) {
    std::string key = decode_location(location);
    std::string value;
    if (!adaptor->GetValue(key, value)) {
        location = nullptr;
        spdlog::warn("get location tag - {} wrapper_id - {}: location doesn't exist", location->tag, location->wrapper_id);
        return false;
    }
    location = new location_t;
    try {
        std::string stat_value;
        nlohmann::json json = nlohmann::json::parse(value);
        json.at("tag").get_to(location->tag);
        json.at("wrapper_id").get_to(location->wrapper_id);
        json.at("stat_value").get_to(stat_value);
        const struct stat* stat = reinterpret_cast<const struct stat *>(stat_value.data());
        std::memcpy(&location->stat, stat, sizeof(struct stat));
    } catch (const std::exception& e) {
        location = nullptr;
        spdlog::error("get location tag - {} wrapper_id - {}: unresolved data format", location->tag, location->wrapper_id);
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("get location: {}", location->debug());
    }
    return true;
}

bool put_location(LevelDBAdaptor* adaptor, location_t* location) {
    if (location == nullptr) {
        spdlog::error("put location: location doesn't exist");
        exit(1);
    }
    if (ENABELD_LOG) {
        spdlog::info("put location: {}", location->debug());
    }
    std::string key = decode_location(location);
    std::string stat_value = std::string(reinterpret_cast<const char*>(&location->stat), sizeof(struct stat));
    nlohmann::json json = nlohmann::json{{"tag", location->tag},
                                          {"wrapper_id", location->wrapper_id},
                                          {"stat_value", stat_value}};
    std::string value = json.dump();
    if (!adaptor->Insert(key, value)) {
        delete location;
        spdlog::error("put location tag - {} wrapper_id - {}: kv store interanl error", location->tag, location->wrapper_id);
        exit(1);
    }
    delete location;
    return true;
}

bool get_range_relations(LevelDBAdaptor* adaptor, std::vector<relation_t> &relations) {
    relations.clear();
    return true;
}

}