#pragma once

///////////////////////////////////////////////////////////////////////////////////////////
// shared resource
namespace vml
{
	namespace utils
	{

		class list_of_parms
		{
			private:

				std::unordered_map<uint32_t, uint32_t> map{};

			public:

				uint32_t operator[](uint32_t key)
				{
					auto it = map.find(key);
					if (it != map.end())
						return it->second;
					vml::os::Message::Error("List of Parms : no such key");
					exit(1);
				}

				list_of_parms(const std::initializer_list<uint32_t>& list)
				{
					for (auto it = list.begin(); it != list.end(); ++it)
						map[*it] = *it;
				}

				~list_of_parms()
				{
				}

		};

	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// SharedResource Node

namespace vml
{
	namespace utils
	{

		class SharedResource
		{
			private:

				friend class SharedResourceStore;

			protected:

				// ----------------------------------------------------------------
				// protected data

				std::string ResourceFileName;
				int			Instances;

			public:

				// ----------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				SharedResource(const SharedResource& sharedresource) = delete;

				// ----------------------------------------------------------------
				// overload operator = is private
				// no copies allowed since classes
				// are referenced

				SharedResource& operator = (const SharedResource& sharedresource) = delete;

				//-----------------------------------------------------------------------------------
				// class is non mmovable

				SharedResource(SharedResource&& other) = delete;
				SharedResource& operator=(SharedResource&& other) = delete;

				// --------------------------------------------------------------------------------
				// get resource filename

				const std::string& GetResourceFileName() const
				{
					return ResourceFileName;
				}

				// --------------------------------------------------------------------------------
				// get resource instances count

				int GetInstancesCount() const
				{
					return Instances;
				}

				// -----------------------------------------------------------------------
				// dump info

				const std::string Dump()
				{
					std::string text;
					text =  std::format("Full path filename : {}\n", ResourceFileName.c_str());
					text += std::format("File directory     : {}\n", vml::strings::SplitPath::GetDirectory(ResourceFileName).c_str());
					text += std::format("File extension     : {}\n", vml::strings::SplitPath::GetExtension(ResourceFileName).c_str());
					text += std::format("File path          : {}\n", vml::strings::SplitPath::GetDrive(ResourceFileName).c_str());
					text += std::format("Instances : {}\n", Instances);
					return text;
				}

				// --------------------------------------------------------------------------------
				// ctor / dtor

				SharedResource(const std::string& key, const std::initializer_list<uint32_t> &il)
				{
					if (key.empty())
						vml::os::Message::Error("SharedResource instance : Null SharedResource name is not allowed");

					ResourceFileName = key;
					Instances		 = 0;
				}

				virtual ~SharedResource()
				{
				}

		};

	}  // end of namespace SharedResources

} // end of namespace vml

/////////////////////////////////////////////////////////////////////////////////////////
// SharedResource manager

namespace vml
{
	namespace utils
	{

		class SharedResourceStore
		{

			private:

				// --------------------------------------------------------------------------------
				// private data

				std::string										 Name;
				uint32_t										 PreferencesFlags;
				std::unordered_map<std::string, SharedResource*> Cache{};

				// --------------------------------------------------------------------------------
				// delete all cache content

				void ReleaseAll()
				{
					if (IsVerbose())
					{
						if (Cache.size() == 0) {
							vml::utils::Logger::GetInstance()->Info("Store : "+ Name + " : No more resources for this store");
						}
						else {
							vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Releasing");
						}
					}

					// force release of all elements in the cache

					for (auto it = Cache.begin(); it != Cache.end(); ++it)
					{
						SharedResource* item = it->second;

						if (IsVerbose()) {
							vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Relasing " + item->GetResourceFileName() + " , instances : " + std::to_string(item->GetInstancesCount()));
						}

						delete it->second;
					}

					Cache.clear();

				}

			public:

				//---------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				SharedResourceStore(const SharedResourceStore& SharedResourcestore) = delete;

				//---------------------------------------------------------------------
				// overload operator is private, 
				// no copies allowed since classes
				// are referenced

				SharedResourceStore& operator = (const SharedResourceStore& SharedResourcestore) = delete;

				// --------------------------------------------------------------------------------
				// adds new element to the cache

				template < class T>
				T* Load(const std::string& fullfilename, const std::initializer_list<uint32_t> &il)
				{
					if (fullfilename.empty())
						vml::os::Message::Error("Store : Null filename");

					// checks if resource is already loaded 

					auto it = Cache.find(fullfilename);

					// if not allocate new resource

					if (it == Cache.end())
					{
						// allocate new resource

						T* item = new T(fullfilename,il);

						// insert in cahce

						Cache.insert(std::pair<std::string, T*>(fullfilename, item));

						// increase instance counter

						item->Instances++;

						// log message out

						if (IsVerbose()) {
							vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Adding " + fullfilename + " , instances : " + std::to_string(item->GetInstancesCount()));
						}

						// return newly creeated item

						return item;
					}

					// if resource is already present, icrease reference count and return its pointer

					it->second->Instances++;

					// log message out

					if (IsVerbose()) {
						vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Adding " + fullfilename + " , instances : " + std::to_string(it->second->GetInstancesCount()) + " * Already Loaded *");
					}

					// return pointer to asset

					return static_cast<T*>(it->second);
				}

				// --------------------------------------------------------------------------------
				// remove element from database

				bool UnLoad(const std::string& filename)
				{
					if (filename.empty())
						vml::os::Message::Error("Store : Null filename when unloading");

					// checks if resource is loaded 

					auto it = Cache.find(filename);
					
					// if not , erase resource if its
					// reference values reaches zero

					if (it != Cache.end())
					{
						// cache resource

						SharedResource* item = it->second;

						if (IsVerbose()) {
							vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Unload " + filename + " , instances : " + std::to_string(item->GetInstancesCount()));
						}

						// reduce instances

						item->Instances--;

						// if instances reaches zero, then erase it

						if (item->Instances == 0)
						{

							if (IsVerbose()) {
								vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Unload : No more references for : " + filename + " , instances : " + std::to_string(item->GetInstancesCount()));
							}

							// call resource destrctor

							delete (it->second);

							// remvoe resource from cache

							Cache.erase(it);

							return true;
						}
						
					}
					else
					{
						if (IsVerbose())
						{
							vml::utils::Logger::GetInstance()->Info("Store : " + Name + " : Couldn't find : " + filename);
						}
					}

					return false;
				}

				// --------------------------------------------------------------------------------
				// dumps content of cache map

				const std::string Dump() const
				{
					std::string text = Name + " : Dumping cache\n";

					for (auto it = Cache.begin(); it != Cache.end(); ++it)
					{
						SharedResource* item = it->second;

						text += item->ResourceFileName + " : instances : " + std::to_string(item->Instances) + "\n";
					}

					return text;
				}

				// --------------------------------------------------------------------------------
				// gets database name

				const std::string& GetName() const
				{
					return Name;
				}

				// --------------------------------------------------------------------------------
				// returns if verbose flag set  to true

				bool IsVerbose() const
				{
					return vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::VERBOSE);
				}

				// --------------------------------------------------------------------------------
				// returns if verbose flag set  to false

				bool IsQuiet() const
				{
					return !vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::VERBOSE);
				}

				// --------------------------------------------------------------------------------
				// ctor / dtor

				[[nodiscard("AssetStore Without Instance!")]] SharedResourceStore(const std::string& name, uint32_t flags = vml::utils::PreferencesFlags::VERBOSE)
				{
					// check if we have a valid name 

					if (name.empty())
						vml::os::Message::Error("Store : Name cannot be empty");

					// sets SharedResource store name

					Name = name;

					// sets flags

					PreferencesFlags = flags;

					// validate preferences flags

					if (IsVerbose() && IsQuiet())
						vml::os::Message::Error("Store : Cannot set both verbose and quiet flags");

					// set verbose flag if needed

					if (IsVerbose()) {
						vml::utils::Logger::GetInstance()->Info("Store : Initting Store : " + Name);
					}

				}

				~SharedResourceStore()
				{
					ReleaseAll();
				}

		};

	}  // end of namespace SharedResources

} // end of namespace vml

/* example of usage
 
	 ////////////////////////////////////////////////////////////////////////

	class A : public vml::utils::SharedResource
	{

		public:

			int a = 1024 * 2;

			A(const std::string& name) :vml::utilss::SharedResource(name)
			{}

			~A()
			{}

	};

	class B : public vml::utils::SharedResource
	{

		public:

		int b = 1024 * 4;

		B(const std::string &name) :vml::utils::SharedResource(name)
		{}

		~B()
		{}

	};

	// code fragment

	vml::utils::SharedResourceStore* s = app->RegisterStore("Store0");

	A* aptr;
	B* bptr;

	aptr = s->Load<A>("ciao A");
	aptr = s->Load<A>("ciao A_1");
	aptr = s->Load<A>("ciao A");
	aptr = s->Load<A>("ciao A_0");

	bptr = s->Load<B>("ciao B");
	bptr = s->Load<B>("ciao B");
	bptr = s->Load<B>("ciao B_2");

	*/

