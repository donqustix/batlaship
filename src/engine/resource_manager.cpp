#include "resource_manager.h"
#include "resource.h"

using battleship::engine::ResourceManager;

ResourceManager::ResourceManager() = default;

ResourceManager::ResourceManager(ResourceManager&&) noexcept = default;

ResourceManager::~ResourceManager() = default;

ResourceManager& ResourceManager::operator=(ResourceManager&&) noexcept = default;

