#ifndef _REFLECTION_HPP_
#define _REFLECTION_HPP_

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

template <typename KeyType, typename BasePointerType, typename... ArgTypes>
class GeneralFactory
{
public:
    virtual ~GeneralFactory() {}

    template <typename DerivedType>
    void Register(const KeyType& key) {
        //_creatorMap[key] = std::bind(&GeneralFactory::CreateImpl<DerivedType>, this, std::forward<ArgTypes>(args)...);
        //_creatorMap[key] = std::bind(&GeneralFactory::CreateImpl<DerivedType>);
        _creatorMap[key] = &GeneralFactory::template CreateImpl<DerivedType>;
    }

    bool Deregister(const KeyType& key) {
        return (_creatorMap.erase(key) == 1);
    }

    bool IsCreatable(const KeyType& key) const {
        return (_creatorMap.count(key) != 0);
    }

    BasePointerType Create(const KeyType& key, ArgTypes&&... args) const {
        return _creatorMap.at(key)(std::forward<ArgTypes>(args)...);
    }

private:
    template <typename DerivedType>
    static BasePointerType CreateImpl(ArgTypes&&... args) {
        BasePointerType pNewObject(new DerivedType(std::forward<ArgTypes>(args)...));
        return pNewObject;
    }

private:
    typedef std::function<BasePointerType(ArgTypes&&...)> CreatorFuncType;
    typedef std::unordered_map<KeyType, CreatorFuncType> CreatorMapType;
    CreatorMapType _creatorMap;
};

//#define REGISTER_DEC_TYPE(NAME) \
//    static DerivedRegister<NAME> reg

//#define REGISTER_DEF_TYPE(NAME) \
//    DerivedRegister<NAME> NAME::reg(#NAME)


#endif // _REFLECTION_HPP_
