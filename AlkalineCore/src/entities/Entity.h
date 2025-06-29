#pragma once

namespace alk
{
    using EntityId = uint32_t;
    /**
     * @brief Entity
     */
    class Entity
    {
    public:
        /**
         * @brief unique identifier
         */
        EntityId id;

        /**
         * @brief Readable name of the entity (dev-only)
         * 
         * @todo generate base on class + id (?)
         */
        std::string name;

        Entity() : id(0), name("None") {}
        Entity(EntityId id, const std::string name) : id(id), name(name) {}

        /**
         * @brief Checks if it's a valid entity reference
         *
         * @return true
         * @return false
         */
        bool IsValid()
        {
            return id > 0;
        }

        /**
         * @brief Returns the entity name - debug purposes only
         * 
         * @return const char* 
         */
        const std::string GetName()
        {
            return name;
        }
    };
}