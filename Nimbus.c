#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAX_RATE 0.05
#define DISCOUNT_RATE 0.10

struct Product {
    int code;
    char name[50];
    float price;
    int stock;
};

void displayProducts(struct Product *p, int n) {
    int i;

    printf("\n%-10s %-20s %-10s %-10s\n",
           "Code", "Name", "Price", "Stock");

    for (i = 0; i < n; i++) {
        printf("%-10d %-20s Rs.%-7.2f %-10d\n",
               p[i].code, p[i].name,
               p[i].price, p[i].stock);
    }
}

int searchProduct(struct Product *p, int n, int code) {
    int i;

    for (i = 0; i < n; i++) {
        if (p[i].code == code)
            return i;
    }

    return -1;
}

void addProduct(struct Product *p, int *n, int capacity) {
    if (*n >= capacity) {
        printf("\nInventory is full!\n");
        return;
    }

    int code;

    printf("Enter product code: ");
    scanf("%d", &code);

    if (searchProduct(p, *n, code) != -1) {
        printf("Product code already exists!\n");
        return;
    }

    p[*n].code = code;

    printf("Enter product name (one word): ");
    scanf("%49s", p[*n].name);

    printf("Enter price: ");
    scanf("%f", &p[*n].price);

    printf("Enter stock quantity: ");
    scanf("%d", &p[*n].stock);

    if (p[*n].price < 0 || p[*n].stock < 0) {
        printf("Invalid price or stock!\n");
        return;
    }

    (*n)++;
    printf("Product added successfully!\n");
}

void updateProduct(struct Product *p, int n) {
    int code, index, choice;

    printf("Enter product code to update: ");
    scanf("%d", &code);

    index = searchProduct(p, n, code);

    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("1. Update price\n");
    printf("2. Update stock\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        float price;
        printf("Enter new price: ");
        scanf("%f", &price);

        if (price < 0) {
            printf("Invalid price!\n");
            return;
        }

        p[index].price = price;
    } else if (choice == 2) {
        int stock;
        printf("Enter new stock: ");
        scanf("%d", &stock);

        if (stock < 0) {
            printf("Invalid stock!\n");
            return;
        }

        p[index].stock = stock;
    } else {
        printf("Invalid choice!\n");
        return;
    }

    printf("Product updated successfully!\n");
}

void generateBill(struct Product *p, int n) {
    int code, qty, index;
    int items = 0;
    float subtotal = 0, discount, tax, total;

    printf("\n--- CUSTOMER BILL ---\n");
    printf("Enter product code and quantity.\n");
    printf("Enter code 0 to finish billing.\n");

    while (1) {
        printf("\nProduct code: ");
        scanf("%d", &code);

        if (code == 0)
            break;

        index = searchProduct(p, n, code);

        if (index == -1) {
            printf("Product not found!\n");
            continue;
        }

        printf("Quantity: ");
        scanf("%d", &qty);

        if (qty <= 0) {
            printf("Invalid quantity!\n");
            continue;
        }

        if (qty > p[index].stock) {
            printf("Insufficient stock! Available: %d\n",
                   p[index].stock);
            continue;
        }

        float amount = p[index].price * qty;
        subtotal += amount;
        p[index].stock -= qty;
        items++;

        printf("%s x %d = Rs.%.2f\n",
               p[index].name, qty, amount);
    }

    if (items == 0) {
        printf("No items billed.\n");
        return;
    }

    /* Conditional discount offer */
    if (subtotal >= 1000)
        discount = subtotal * DISCOUNT_RATE;
    else
        discount = 0;

    /* Tax is calculated after discount */
    tax = (subtotal - discount) * TAX_RATE;
    total = subtotal - discount + tax;

    printf("\n-----------------------------\n");
    printf("Subtotal       : Rs.%.2f\n", subtotal);
    printf("Discount       : Rs.%.2f\n", discount);
    printf("Tax (5%%)       : Rs.%.2f\n", tax);
    printf("-----------------------------\n");
    printf("TOTAL AMOUNT   : Rs.%.2f\n", total);
    printf("-----------------------------\n");
    printf("Thank you for shopping!\n");
}

int main(void) {
    int capacity, n = 0, choice;

    printf("SMART BILLING & INVENTORY SYSTEM\n");
    printf("Enter maximum number of products: ");

    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity!\n");
        return 1;
    }

    /* Dynamic memory allocation */
    struct Product *products =
        malloc(capacity * sizeof(struct Product));

    if (products == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    do {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Add Product\n");
        printf("2. Display Inventory\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Generate Bill\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            free(products);
            return 1;
        }

        switch (choice) {
            case 1:
            
                addProduct(products, &n, capacity);
                break;

            case 2:
                displayProducts(products, n);
                break;

            case 3: {
                int code, index;

                printf("Enter product code: ");
                scanf("%d", &code);

                index = searchProduct(products, n, code);

                if (index != -1) {
                    printf("Code: %d\n",
                           products[index].code);
                    printf("Name: %s\n",
                           products[index].name);
                    printf("Price: Rs.%.2f\n",
                           products[index].price);
                    printf("Stock: %d\n",
                           products[index].stock);
                } else {
                    printf("Product not found!\n");
                }
                break;
            }

            case 4:
                updateProduct(products, n);
                break;

            case 5:
                generateBill(products, n);
                break;

            case 6:
                printf("Exiting system...\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 6);

    /* Release dynamically allocated memory */
    free(products);

    return 0;
}