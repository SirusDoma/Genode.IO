#include <Genode/IO/ResourceContext.hpp>
#include <utility>

namespace Gx
{
    const ResourceContext ResourceContext::Default;

    ResourceContext::ResourceContext() noexcept :
        m_id(),
        m_resources()
    {
    }

    ResourceContext::ResourceContext(std::string id) :
        m_id(std::move(id)),
        m_resources(nullptr)
    {
    }

    ResourceContext::ResourceContext(std::string id, ResourceManager &resources) :
        m_id(std::move(id)),
        m_resources(&resources)
    {
    }

    const std::string &ResourceContext::GetID() const
    {
        return m_id;
    }
}
