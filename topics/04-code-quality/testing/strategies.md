# Testing Strategies

## Introduction
Testing is a crucial aspect of software development that ensures code quality, reliability, and maintainability. This guide covers different testing strategies, their implementation, and best practices.

## Types of Testing

### 1. Unit Testing
```cpp
// Calculator.h
class Calculator {
public:
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
};

// CalculatorTest.cpp
TEST(CalculatorTest, Add) {
    Calculator calc;
    EXPECT_EQ(calc.add(2, 3), 5);
    EXPECT_EQ(calc.add(-1, 1), 0);
    EXPECT_EQ(calc.add(0, 0), 0);
}

TEST(CalculatorTest, Divide) {
    Calculator calc;
    EXPECT_EQ(calc.divide(6, 2), 3);
    EXPECT_THROW(calc.divide(1, 0), std::invalid_argument);
}
```

### 2. Integration Testing
```cpp
// OrderProcessor.h
class OrderProcessor {
public:
    OrderProcessor(Database& db, PaymentGateway& gateway)
        : m_db(db), m_gateway(gateway) {}
    
    void processOrder(Order& order) {
        m_db.saveOrder(order);
        m_gateway.processPayment(order.getPayment());
    }
    
private:
    Database& m_db;
    PaymentGateway& m_gateway;
};

// OrderProcessorTest.cpp
TEST(OrderProcessorTest, ProcessOrder) {
    // Setup
    MockDatabase db;
    MockPaymentGateway gateway;
    OrderProcessor processor(db, gateway);
    Order order;
    
    // Execute
    processor.processOrder(order);
    
    // Verify
    EXPECT_TRUE(db.wasOrderSaved());
    EXPECT_TRUE(gateway.wasPaymentProcessed());
}
```

### 3. System Testing
```cpp
// SystemTest.cpp
TEST(SystemTest, CompleteOrderFlow) {
    // Setup
    Application app;
    app.initialize();
    
    // Execute
    app.login("user", "pass");
    app.addItemToCart("item1");
    app.checkout();
    app.confirmOrder();
    
    // Verify
    EXPECT_TRUE(app.isOrderConfirmed());
    EXPECT_TRUE(app.wasEmailSent());
    EXPECT_TRUE(app.wasPaymentProcessed());
}
```

## Testing Patterns

### 1. Arrange-Act-Assert (AAA)
```cpp
TEST(OrderProcessorTest, ApplyDiscount) {
    // Arrange
    Order order;
    order.setTotal(100);
    DiscountCalculator calculator;
    
    // Act
    calculator.applyDiscount(order);
    
    // Assert
    EXPECT_EQ(order.getTotal(), 90);
    EXPECT_TRUE(order.hasDiscount());
}
```

### 2. Test Doubles
```cpp
// Mock Database
class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(void, saveOrder, (const Order&), (override));
    MOCK_METHOD(Order, getOrder, (int), (override));
};

// Stub Payment Gateway
class StubPaymentGateway : public IPaymentGateway {
public:
    bool processPayment(const Payment& payment) override {
        return true;  // Always succeeds
    }
};

// Fake Repository
class FakeUserRepository : public IUserRepository {
public:
    User findById(int id) override {
        return User(id, "Test User");  // Returns test data
    }
};
```

### 3. Parameterized Tests
```cpp
struct CalculatorTestData {
    double a;
    double b;
    double expected;
};

class CalculatorTest : public ::testing::TestWithParam<CalculatorTestData> {};

TEST_P(CalculatorTest, Add) {
    Calculator calc;
    auto data = GetParam();
    EXPECT_EQ(calc.add(data.a, data.b), data.expected);
}

INSTANTIATE_TEST_SUITE_P(
    CalculatorTests,
    CalculatorTest,
    ::testing::Values(
        CalculatorTestData{2, 3, 5},
        CalculatorTestData{-1, 1, 0},
        CalculatorTestData{0, 0, 0}
    )
);
```

## Best Practices

### 1. Test Organization
```cpp
// Good
class UserServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup
    }
    
    void TearDown() override {
        // Common cleanup
    }
    
    UserService service;
    MockUserRepository repository;
};

TEST_F(UserServiceTest, CreateUser) {
    // Test implementation
}

// Bad
TEST(UserServiceTest, CreateUser) {
    UserService service;
    MockUserRepository repository;
    // Test implementation
}
```

### 2. Test Naming
```cpp
// Good
TEST(UserServiceTest, CreateUser_WithValidData_ShouldSucceed)
TEST(UserServiceTest, CreateUser_WithInvalidEmail_ShouldThrowException)
TEST(UserServiceTest, CreateUser_WithDuplicateUsername_ShouldThrowException)

// Bad
TEST(UserServiceTest, Test1)
TEST(UserServiceTest, CreateUser)
TEST(UserServiceTest, CreateUserTest)
```

### 3. Test Independence
```cpp
// Good
class OrderProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_processor = std::make_unique<OrderProcessor>();
        m_processor->initialize();
    }
    
    std::unique_ptr<OrderProcessor> m_processor;
};

TEST_F(OrderProcessorTest, ProcessOrder1) {
    // Test implementation
}

TEST_F(OrderProcessorTest, ProcessOrder2) {
    // Independent test implementation
}

// Bad
class OrderProcessorTest : public ::testing::Test {
protected:
    OrderProcessor processor;
};

TEST_F(OrderProcessorTest, ProcessOrder1) {
    processor.processOrder(order1);
    // Modifies shared state
}

TEST_F(OrderProcessorTest, ProcessOrder2) {
    // Depends on state from previous test
    processor.processOrder(order2);
}
```

## Common Pitfalls

### 1. Brittle Tests
```cpp
// Bad
TEST(OrderProcessorTest, ProcessOrder) {
    OrderProcessor processor;
    Order order;
    processor.processOrder(order);
    EXPECT_EQ(order.getTotal(), 100);  // Hard-coded value
}

// Good
TEST(OrderProcessorTest, ProcessOrder) {
    OrderProcessor processor;
    Order order;
    order.setItems(createTestItems());
    processor.processOrder(order);
    EXPECT_EQ(order.getTotal(), calculateExpectedTotal(order.getItems()));
}
```

### 2. Testing Implementation Details
```cpp
// Bad
TEST(UserServiceTest, CreateUser) {
    UserService service;
    service.createUser("user", "pass");
    EXPECT_EQ(service.getDatabase().getQueryCount(), 1);  // Testing implementation
}

// Good
TEST(UserServiceTest, CreateUser) {
    UserService service;
    User user = service.createUser("user", "pass");
    EXPECT_EQ(user.getUsername(), "user");  // Testing behavior
}
```

### 3. Incomplete Test Coverage
```cpp
// Bad
TEST(CalculatorTest, Add) {
    Calculator calc;
    EXPECT_EQ(calc.add(2, 2), 4);  // Only tests happy path
}

// Good
TEST(CalculatorTest, Add) {
    Calculator calc;
    EXPECT_EQ(calc.add(2, 2), 4);
    EXPECT_EQ(calc.add(-1, 1), 0);
    EXPECT_EQ(calc.add(0, 0), 0);
    EXPECT_EQ(calc.add(1.5, 2.5), 4.0);
}
```

## Practice Problems

1. Write unit tests for a calculator class
2. Implement integration tests for an order processing system
3. Create parameterized tests for a string utility class
4. Write tests using test doubles (mocks, stubs, fakes)
5. Implement a complete test suite for a user management system

## Interview Questions

1. What are the different types of testing and when should each be used?
2. How do you ensure good test coverage?
3. What are test doubles and when should they be used?
4. How do you handle testing of external dependencies?
5. What makes a good test case?

## Summary
- Write clear, focused tests
- Follow testing patterns and best practices
- Maintain test independence
- Avoid common pitfalls
- Regular test maintenance
- Continuous test improvement 