#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class Package {
private:
    string name;

public:
    Package(string name) : name(name) {}

    string getName() const {
        return name;
    }
};

class Repository {
private:
    string name;
public:
    Repository(string name) : name(name) {}

    string getName() const {
        return name;
    }

    // Assume this performs a slow NETWORK request.
    //
    // If the package exists:
    //     returns the Package
    //
    // If the package does not exist:
    //     throws an exception
    Package getPackage(const string& packageName) {
        // Implementation provided by interviewer.
        throw runtime_error("Not implemented");
    }
};

class PackageManager {
private:
    vector<Repository*> repositories;
    unordered_map<string, Repository*> cache;

    /*
        Helper function.

        Starts an asynchronous request to a repository
        and immediately returns a Future representing
        the eventual Package result.
    */
    future<Package> getPackageFuture(
        Repository* repository,
        const string& packageName
    ) {
        return async(
            launch::async,
            [repository, packageName]() {
                return repository->getPackage(packageName);
            }
        );
    }

public:
    PackageManager(vector<Repository*> repositories)
        : repositories(repositories) {}

    /*
        CURRENT IMPLEMENTATION

        This works correctly, but is inefficient.

        Improve this implementation.
    */
    Package getPackage(const string& packageName) {
        auto it = cache.find(packageName);

        if(it != cache.end()) {
            Repository* repo = it->second;
            return repo->getPackage(packageName);
        }
        
        // future 가 어디 출신인지 알기 위해서 Repository* 도 같이 저장
        vector<pair<Repository*, future<Package>>> futures;

        for (Repository* repository : repositories) {
            futures.emplace_back(
                repository,
                getPackageFuture(repository, packageName)
            );
        }

        for(auto& [repo, future] : futures) {
            try {
                Package p = future.get();
                cache[packageName] = repo; //cache

                return p;
            } catch (const exception& e) {
                continue;
            }
        }

        throw runtime_error("Package not found");
    }
};