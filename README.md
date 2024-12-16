# Genode.IO #

> [!important]
> This repository has been archived because it has been superseded by the full [Genode](https://github.com/SirusDoma/Genode) repository.

Genode.IO is a module that provides Resource Management functionalities for a game.
It is one of subset modules of my in-house game engine called **Genode** (**G**ame **E**ngi**N**e **O**n **DE**mand).

As the name suggest, this project is on the contranary side of other common engine or framework; it is not intended for general use.
It will only evolve or change to satisfy _demand_ of my game needs, which favor of certain convention, pattern or even my personal preferences.

This applies to the entire modules of Genode including this IO module. As such, You may find some part of API isn't flexible enough for your need (or inefficient or doesn't make any sense to you).
Still, this project should be fit for wide-range of use cases especially simple ones.

Treat this as a heed of warning before integrating this module within your production code.

#### Note ####
Some part of API are actually excluded from the original Genode IO module. 
It's either because incomplete, too tightly coupled with the game or simply not ready for public consumption (draft codes, design flaw, buggy, major performance issue, etc.)

## Building the Project ##
Currently, the project is focuses on Windows platform, but it should be perfectly working on other platforms as well. 
You just have to adjust SFML installation command below (and perhaps `CMakeList.txt`).

Use [vcpkg](https://github.com/Microsoft/vcpkg) to simplify project dependencies installation.
Follow vcpkg installation instruction then use following command to install SFML on Windows:

```shell
vcpkg install sfml:x86-windows-static
``` 

All dependencies that required by SFML (such as FreeType, Vorbis, OpenAL, etc.) are automatically installed along the way.
After installation complete, configure the project.

### Visual Studio ###
Override auto-detected triplet using MSBuild config `VcpkgTriplet` and `VcpkgEnabled`. Make sure following lines exists in `.vcxproj` file:

```xml
<PropertyGroup Label="Globals">
  <!-- .... -->
  <VcpkgTriplet Condition="'$(Platform)'=='Win32'">x86-windows-static</VcpkgTriplet>
  <VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
</PropertyGroup>
```

### CMake ###
You need to specify toolchain and target triplet using `-DCMAKE_TOOLCHAIN_FILE` and `-DVCPKG_TARGET_TRIPLET` respectively.
CMake Toolchain File can be found under `<vcpkg_path>/scripts/buildsystems/vcpkg.cmake`.

Use following command to generate cmake build files:

```shell
# Generate build files, make sure to set the output to "build/cmake-build-<build config>"
cmake -B build/cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=<vcpkg_path>/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x86-windows-static .

# Build the project
cmake --build build/cmake-build-debug --target Genode
```

In addition to build cmake manually using terminal, you can configure these settings with **CLion** under `Settings` > `Build, Execution, Deployment` > `CMake`.

## Usage ##

### IResourceLoader ###

This module defines how you should deserialize your resources by implementing `Gx::IResourceLoader` interface. It is up to you to decide what types that considered as resources.

It also up to your implementation whether the lifetime of certain type of resources should be managed or not.
However, it shouldn't affect the implementation of deserialization process.

By default, this module provides 3 built-in resource loader, they're `Gx::TextureLoader`, `Gx::FontLoader` and `Gx::SoundBufferLoader`. 
They also automatically registered within `Gx::ResourceLoaderFactory` so you don't have to register them manually.

#### Basic Deserialization ####

A valid `Gx::IResourceLoader` implementation must override `LoadFromFile`, `LoadFromMemory` and `LoadFromStream` function
that describe how a resource should be loaded from 3 different sources respectively.

Typically, a `Gx::IResourceLoader` for `sf::Texture` will look something like this:

```c++
class TextureLoader : public Gx::IResourceLoader
{
    public:
        std::unique_ptr<sf::Texture> LoadFromFile(const std::string &path, Gx::ResourceContext &_)
        {
            auto texture = std::make_unique<sf::Texture>();
            if (!texture->loadFromFile(path))
                return nullptr;
            
            return texture;
        }
        
        // Do the same for LoadFromMemory and LoadFromStream ...
};

// Usage:
TextureLoader loader;
auto texture = loader.LoadFromFile("/path/to/texture/metadata.json", Gx::ResourceContext::Default);
```

#### Resource Metadata ####

You may notice that `LoadFromFile` function did not provide additional parameter to supply extra information for constructing a resource.
For example, it doesn't have optional parameter to provide area of the texture image to load.

This is something to be expected, the engine encourage you to use some sort of metadata / definition file for deserialization process
that describe the properties of a particular resource.

For example, the `path` parameter should specify the metadata / definition file instead, not the path of actual raw resource file itself.

```c++
class CustomTextureLoader : public Gx::IResourceLoader
{
    public:
        std::unique_ptr<sf::Texture> LoadFromFile(const std::string &path, Gx::ResourceContext &_)
        {
            auto texture = std::make_unique<sf::Texture>();
            auto metadata = // ... load metadata from "path" which basically a json/xml file
            if (!texture->loadFromFile(metadata.GetTexturePath(), metadata.GetArea()))
                return nullptr;
            
            return texture;
        }
        
        // Do the same for loadFromMemory and loadFromStream ...
};

// Usage:
CustomTextureLoader loader;
auto texture = loader.LoadFromFile("/path/to/texture/metadata.json", Gx::ResourceContext::Default);
```

This will reduce the number of constants when loading resources and enable you to build various complex resources which can be useful for many scenario.
However, it is perfectly fine for trivial resource to not have metadata / definition.

#### Resource Dependency ####

Some type of resources may depend on other resource. You can either use `Gx::IResourceLoader` manually to load the resource,
or use provided `Gx::ResourceContext` to solve resource dependency if you're using `Gx::ResourceManager` to manage your resources.

Note that an independent loader without `Gx::ResourceManager` will not be able to utilize the context,
Otherwise it will throw `Gx::ResourceLoadException`. In such case, the only choice you have is to load resource dependency manually.

It is also important to remember that **you have to manage resource dependency lifetime by yourself** when
loading resource dependency via `Gx::IResourceLoader` manually. This is important especially for most of SFML resources.

Consider following example where `SpriteLoader` need to load corresponding `sf::Texture`:
```c++
class SpriteLoader : public Gx::IResourceLoader
{
    public:
        std::unique_ptr<sf::Texture> LoadFromFile(const std::string &path, Gx::ResourceContext &ctx)
        {
            auto sprite = std::make_unique<sf::Sprite>();
            auto metadata = // ... load metadata from "path" which basically a json/xml file
            
            // With IResourceLoader to load resource dependency manually
            // WARNING: You have to manage "texture" lifetime, otherwise invalid sprite returned
            auto loader  = TextureLoader(); // or Gx::ResourceLoaderFactory::CreateResourceLoaderFor<sf::Texture>();
            auto texture = loader->LoadFromFile(metadata.GetTexturePath(), ctx);
            if (texture)
                return nullptr;
            sprite->setTexture(&texture.get());
            
            // Or with Resource Manager to get resource dependency that managed by ResourceManager
            // In this case you don't have to manage texture lifetime.
            auto& texture = ctx.Acquire<sf::Texture>(metadata.GetTextureID(), metadata.GetTexturePath());
            sprite->setTexture(texture);
            
            // Set the rest of attributes from metadata
            sprite->setPosition(metadata.GetPosition());
            sprite->setRotation(metadata.GetRotation());
            // And so on..
            
            // If you use `Gx::IResourceLoader` to load resource dependency, "texture" need to alive beyond this scope,
            // Otherwise this function will return broken sf::Sprite.
            return sprite;
        }
        
        // Do the same for loadFromMemory and loadFromStream ...
};

// Register resource loaders
Gx::ResourceLoaderFactory::Register<sf::Texture, TextureLoader>();
Gx::ResourceLoaderFactory::Register<sf::Sprite, SpriteLoader>();

// Usage (with standalone Gx::IResourceLoader):
auto loader = Gx::ResourceLoaderFactory::CreateResourceLoaderFor<sf::Sprite>();
auto sprite = loader.Load("/path/to/sprite/metadata.json", Gx::ResourceContext::Default);

// Usage (with Gx::ResourceManager):
Gx::ResourceManager resources;
auto& sprite = resources.AddFromFile<sf::Sprite>("/path/to/sprite/metadata.json"); // Gx::ResourceContext is handled by ResourceManager
```

See [ResourceManager](#resource-manager) for further information about managing your resources with `Gx::ResourceManager`.

### Resource Container ###

`Gx::ResourceContainer` template class provides central point to store, access and destroy your resources.
The template define type of resource that can be stored inside `Gx::ResourceContainer`.

By using `Gx::ResourceContainer`, you agree to transfer the ownership of resource to `Gx::ResourceContainer`.
As such, whenever `Gx::ResourceContainer` gets destroyed, all resources inside it will be destroyed as well.

There is no mechanism to provide you to regain or share the ownership of the resource.
Sharing ownership or Dynamic Resource Allocation is considered micro-optimization that avoided by Genode.
The idea is to allocate the resource when they actually used and de-allocate them when no longer needed, as the result memory usage can be keep at minimum.

However, Dynamic de-allocation by sharing ownership can be hard to maintain, improper implementation may lead to
memory leak as the resource potentially never gets destroyed in certain cases and its often unclear when the resource gets destroyed since everything become implicit.

As the result, it may lead to chaos and confusion as the game / application grows.
Even if you don't care when a resource gets allocated or de-allocated, we still trade precious CPU Utilization for (often small and trivial) Memory Usage.  

The deserialization could utilize CPU considerably when dealing with a lot of resources
(especially when deserialization process involve decryption, decompression, etc.), while Memory usage tends to be small for a mere 2D game.

#### Storing Resource ####

`Gx::ResourceContainer` is solely container, it doesn't describe how you load your resource,
such responsibility is belongs to `Gx::IResourceLoader`.

However, it defines how you store your resource, such as how you identify the resource and
what behavior you expect when same resource identifier is already exists inside the container.

Each resource should be able identified by `std::string` which represents the resource names.
Additionally, you can also set certain behavior when resource id clash with existing resource.

```c++
auto container = Gx::ResourceContainer<sf::Texture>();
auto loader    = Gx::ResourceLoaderFactory::CreateResourceLoaderFor<sf::Texture()>();
auto resource  = loader->LoadFromFile("/some/path/to/texture.png");

if (resource)
    container.Store("myTextureID", resource);
```

In order to set behavior when there's clashing resource id between resources, use third optional parameter which accept `Gx::CacheMode`.
There's 3 `Gx::CacheMode` available to use:

1. `Gx::CacheMode::Allocate`: Resource must be newly allocated, throw `Gx::ResourceStoreException` when resource id clash occur.
2. `Gx::CacheMode::Update`: Existing resource will be replaced with newly given resource whenever resource id clash occur.
3. `Gx::CacheMode::Reuse`: Will return existing resource and ignore the newly given resource whenever resource id clash occur.

```c++
auto container = Gx::ResourceContainer<sf::Texture>();
auto foo = // ...
auto bar = // ...

auto& a = container.Store("myTextureID", foo, CacheMode::Allocate);
auto& b = container.Store("myTextureID", bar, CacheMode::Reuse);

// Result:
// foo != bar
// a == b
// &a == foo
// &b == foo
// &b != bar
```

Additionally, you can provide deserializer closure to defer resource loading.
This will avoid resource loading when certain `Gx::CacheMode` in use which should improve your application / game performance.

```c++
auto loader = Gx::ResourceLoaderFactory::CreateResourceLoaderFor<sf::Texture()>();
container.Store("myTextureID", [&] () {
    // Following block will not run if "myTextureID" exists within container due to "CacheMode::Reuse"
    auto data = // ...
    auto size = // ...
    return loader->LoadFromMemory(data, size, Gx::ResourceContext::Default); 
}, CacheMode::Reuse);
```

#### Fetching & Destroying Resource ####

Once resource is stored within the container, use `Find` and `Destroy` to locate and destroy resource respectively.

```c++
// Finding resource
auto resource = container.Find<sf::Texture>("myTextureID");
if (!resource)
    return;

// Destroying resource by ID
bool success = container.Destroy("myTextureID");

// Or by using reference to resource
bool success = container.Destroy(&resource);
```

### Resource Manager ###

It is often tedious to manually manage a collection of `Gx::ResourceContainer` especially when the game / application
use various types of resources. This where `Gx::ResourceManager` class coming to play, it provides `Gx::ResourceContainer` orchestration
as well as resource instantiation at one place. It also automatically resolve `Gx::IResourceLoader` for you so you don't have to load your resource manually.

The lifetime of resource containers shares with `Gx::ResourceManager`.
Therefore, when `Gx::ResourceManager` destroyed, all resource containers it handles will be destroyed as well.

#### Loading and Storing Resource ####

Unlike resource container, `Gx::ResourceManager` handles resource loading for you from 3 standard `Gx::IResourceLoader` sources.
`AddFromFile`, `AddFromMemory` and `AddFromStream` correspond to `Gx::IResourceLoader` load functions.

Additionally, use `AddFromDeserializer` to defer / avoid deserialization process when using certain `Gx::CacheMode`, just like `Gx::ResourceContainer`.

Note that the ID is unique for each resource types that `Gx::ResourceManager` manage. The uniqueness is not shared between resource types.
As such, it is possible to have same Resource ID for different type of resource.

```c++
// You still have to register resource loaders
Gx::ResourceLoaderFactory::Register<sf::Texture, TextureLoader>();

auto resources = Gx::ResourceManager();
auto &texture  = resources.AddFromFile<sf::Texture>("myTextureID", "/some/path/to/texture.png", CacheMode::Reuse);
```

#### Instantiation ####

Sometimes, you want to use your resource as a template or _prefab_. In other words, you don't want to use or modify the resource  directly but rather you want a copy of it.

Use `Instantiate` to copy existing resource from `Gx::ResourceManager` and return it as `std::unique_ptr`. The ownership of the instantiated resource is belong to you, not the `Gx::ResourceManager`.
Therefore, it will be not destroyed when `Gx::ResourceManager` destroyed. Also, you will **not** be able to locate the resource via `Find<R>`. 

Additionally, `Instantiate` function has some function overloads to provide
resource loading shortcut when template or _prefab_ is not exists within the `Gx::ResourceManager`

```c++
auto resources = Gx::ResourceManager();

// Get a copy of "mySpriteID", return nullptr if "mySpriteID" is not exists within Gx::ResourceManager
auto sprite = resources.Instantiate<sf::Sprite>("mySpriteID");

// Alternatively, load it from file when it not exists
auto sprite = resources.Instantiate<sf::Sprite>("mySpriteID", "path/to/metadata.json");

// Or from memory
auto data = // ...
auto size = // ...
auto sprite = resources.Instantiate<sf::Sprite>("mySpriteID", data, size);

// Or from stream
sf::InputStream stream = // ...
auto sprite = resources.Instantiate<sf::Sprite>("mySpriteID", stream);

// Or use deserialization function
auto sprite = resources.Instantiate<sf::Sprite("mySpriteID", [&] () {
    // Following block will not run if "myTextureID" exists within container due to "CacheMode::Reuse"
    auto data = // ...
    auto size = // ...
    return loader->LoadFromMemory(data, size, Gx::ResourceContext::Default); 
});
```

#### Fetching & Destroying Resource ####

`Gx::ResourceManager` also has `Find` and `Destroy` functions that correspond to `Gx::ResourceContainer`.

```c++
// Finding resource
auto resource = container.Find<sf::Texture>("myTextureID");
if (!resource)
    return;

// Destroying resource by ID
bool success = container.Destroy<sf::Texture>("myTextureID");

// Or by using reference to resource
bool success = container.Destroy(&resource);
```

### FileSystem ###

In addition to resource management, this module also provides an extremely simple FileSystem virtualization for easy access to resources.
Implementing `Gx::IFileSystem` can be used to enable access to resources from desired sources, such as from bundled asset file or network.

A valid implementation of `Gx::IFileSystem` has to implement `Open()`, `Read()` and `IsExists()`.
The latter use to check whether the given file name is exists in FileSystem  which can be used to indicate whether the FileSystem has ability to deal with the file.

Additionally, the module provide `Gx::LocalFileSystem` to provide access to file located in the disk.

```c++
// Initializes instance and set root directory
auto localFs = Gx::LocalFileSystem("./some/path/to/assets");

// Mount FileSystem
Gx::FileSystem::Mount(localFs);

// This will open "./some/path/to/assets/Interface.opi" (assuming the actual file exists)
auto stream = Gx::FileSystem::Open("Interface.opi");
```

You can also combine this with the `Gx::IResourceLoader` to resolve appropriate filename.

Note that `GetFullName` will only resolve filename that exists in the disk. 
Therefore, the resolve process will only utilize `Gx::LocalFileSystem` or any other file systems that inherit it.

```c++
class TextureLoader : public Gx::IResourceLoader
{
    public:
        std::unique_ptr<sf::Texture> LoadFromFile(const std::string &path, Gx::ResourceContext &_)
        {
            // Resolve filename, if file is not exists for all mounted FileSystems, it will return original string
            auto fileName = Gx::FileSystem::GetFullName(path);

            auto texture = std::make_unique<sf::Texture>();
            if (!texture->loadFromFile(fileName))
                return nullptr;
            
            return texture;
        }
};
```

## License ##
This is an open-sourced library licensed under the [MIT license](http://github.com/SirusDoma/Genode.IO/blob/main/LICENSE).
