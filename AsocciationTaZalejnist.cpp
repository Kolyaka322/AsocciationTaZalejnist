#include <iostream>
#include <string>
#include <vector>

class Order {
private:
    int orderId;
    std::string address;

public:
    Order(int id, std::string addr) : orderId(id), address(addr) {}

    void printInfo() const {
        std::cout << "Замовлення #" << orderId << ", Адреса: " << address << std::endl;
    }

    int getId() const { return orderId; }
};

class Courier {
private:
    std::string name;
    // Зберігаємо вказівники на замовлення (агрегація: об'єкти існують окремо)
    std::vector<const Order*> orders;

public:
    Courier(std::string courierName) : name(courierName) {}

    void assignOrder(const Order* order) {
        if (order != nullptr) {
            orders.push_back(order);
        }
    }

    void showDeliveries() const {
        std::cout << "Кур'єр: " << name << "\nПризначені замовлення:\n";
        if (orders.empty()) {
            std::cout << "  (немає замовлень)\n";
            return;
        }
        for (const auto* order : orders) {
            std::cout << "  - ";
            order->printInfo();
        }
    }
};


class Dough {
private:
    std::string type; // наприклад, "тонке" або "пухке"
    int sizeCm;       // розмір у см

public:
    Dough(std::string t, int size) : type(t), sizeCm(size) {}

    void printInfo() const {
        std::cout << "Тісто: " << type << ", Розмір: " << sizeCm << " см" << std::endl;
    }
};

class Pizza {
private:
    std::string name;
    Dough dough; // Композиція: об'єкт тіста є невіддільною частиною піци (значення, не вказівник)

public:
    // Конструктор ініціалізує внутрішнє тісто через список ініціалізації
    Pizza(std::string pizzaName, std::string doughType, int doughSize)
        : name(pizzaName), dough(doughType, doughSize) {
    }

    void printReceipt() const {
        std::cout << "Піца: " << name << std::endl;
        dough.printInfo();
    }
};

int main() {
    system("chcp 1251>null");
    // Налаштування для коректного виведення кирилиці (за потреби)
    std::setlocale(LC_ALL, "Ukrainian");

    std::cout << "=== ДЕМОНСТРАЦІЯ ЗАВДАННЯ 1: АГРЕГАЦІЯ ===" << std::endl;
    {
        // 1. Створюємо замовлення окремо у пам'яті
        Order order1(101, "вул. Хрещатик, 1");
        Order order2(102, "вул. Шевченка, 15");

        {
            // Створюємо кур'єра в локальній області видимості
            Courier courier("Олексій");

            // 3. Передаємо замовлення кур'єру за вказівником
            courier.assignOrder(&order1);
            courier.assignOrder(&order2);

            courier.showDeliveries();
        } // 4. Блок завершується — об'єкт `courier` знищується, але замовлення продовжують жити!

        std::cout << "\n[Перевірка]: Кур'єр завершив роботу (знищений), але самі замовлення далі існують:\n";
        order1.printInfo();
        order2.printInfo();
    }

    std::cout << "\n========================================\n\n";

    std::cout << "=== ДЕМОНСТРАЦІЯ ЗАВДАННЯ 2: КОМПОЗИЦІЯ ===" << std::endl;
    {
        // 1. Створюємо піцу (параметри тіста передаються напряму в її конструктор)
        Pizza pizza("Пепероні", "тонке", 30);

        // 2. Виводимо чек
        pizza.printReceipt();

        std::cout << "\n[Перевірка]: Клієнт не може створити тісто окремо і передати його ззовні. "
            << "\nТісто народжується разом з піцою і знищується автоматично при знищенні піци." << std::endl;
    } // Об'єкт `pizza` (і разом з ним внутрішній об'єкт `dough`) знищуються тут.

    return 0;
}