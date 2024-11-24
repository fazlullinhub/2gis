#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

class LocationFinder {
public:
    LocationFinder(const std::string& apiKey) : apiKey(apiKey) {}

    void findNearby(const std::string& address, const std::string& type) {
        std::string url = "https://catalog.api.2gis.com/3.0/items";
        std::string query = "?key=" + apiKey + "&q=" + type + "&point=" + address + "&fields=items.name,items.address,items.point&limit=5";

        cpr::Response response = cpr::Get(cpr::Url{ url + query });

        if (response.status_code != 200) {
            std::cerr << "Ошибка при запросе: " << response.error.message << std::endl;
            return;
        }

        auto jsonResponse = nlohmann::json::parse(response.text);
        if (jsonResponse["result"]["items"].empty()) {
            std::cout << "Не найдено объектов по вашему запросу." << std::endl;
            return;
        }

        for (const auto& item : jsonResponse["result"]["items"]) {
            std::cout << "Название: " << item["name"] << std::endl;
            std::cout << "Адрес: " << item["address"] << std::endl;
            std::cout << std::endl;
        }
    }

private:
    std::string apiKey;
};

int main() {
    std::string apiKey;
    std::cout << "Введите ваш API ключ 2GIS: ";
    std::cin >> apiKey;

    std::string address;
    std::string type;

    std::cout << "Введите адрес (например, '59.9343,30.3351' для Санкт-Петербурга): ";
    std::cin.ignore();
    std::getline(std::cin, address);

    std::cout << "Введите тип заведения (например, 'кафе', 'ресторан' или 'автосервис'): ";
    std::getline(std::cin, type);

    LocationFinder finder(apiKey);
    finder.findNearby(address, type);

    return 0;
}