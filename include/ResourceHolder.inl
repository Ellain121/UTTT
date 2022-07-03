#include <cassert>
#include <iostream>

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, 
                                            std::unique_ptr<Resource> resource)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    std::unique_ptr<Resource> resource(new Resource);

    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error("ResourceHolder::load - failed to load " + filename);
    }
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, 
                        const std::string& filename, const Parameter& secondParam)
{
    std::unique_ptr<Resource> resource(new Resource);

    if (!resource->loadFromFile(filename, secondParam))
    {
        throw std::runtime_error("ResourceHolder::load(secondParam) - failed to load " + filename);
    }
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

