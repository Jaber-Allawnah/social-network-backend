# Social Network Backend

A console-based social network backend built in modern C++ with MySQL. The project applies a layered architecture to separate application flow, business rules, data access, database management, and utility concerns.

## Features

### Authentication
- Register a new user
- Log in with an existing account
- Secure password hashing and verification

### Posts
- Create posts
- Update and delete owned posts
- View a specific post
- View a user's posts
- Search within a user's posts
- View an accessible post feed

### Comments
- Add comments to posts
- Update and delete owned comments
- View comments on a post

### Likes
- Like and unlike posts
- View users who liked a post
- Prevent duplicate likes

### Following
- Send follow requests
- Accept or reject follow requests
- View sent and received follow requests
- Unfollow users
- View followers and following
- Prevent users from following themselves
- Prevent duplicate follow relationships

### Access Control
A user can always access and interact with their own posts. Access to another user's posts is controlled by the follow relationship. Ownership checks are applied when updating or deleting posts and comments.

## Architecture

The project follows a layered design:

```text
main
  |
  v
Application / Menu Layer
  |
  v
Service Layer
  |
  v
Repository Interfaces
  |
  v
Repository Implementations
  |
  v
Database / MySQL
```

### Application Layer

Handles console interaction and application flow.

The menu system is divided by responsibility:

- `LoggedOutMenu` — registration and login
- `LoggedInMenu` — authenticated-user navigation
- `PostMenu` — post operations
- `CommentMenu` — comment operations
- `LikeMenu` — like operations
- `FollowMenu` — followers, following, and follow requests
- `ConnectionMenu` — connection-related navigation
- `Menu` — common menu behaviour

### Service Layer

Contains the business rules of the application:

- `UserService`
- `PostService`
- `CommentService`
- `LikeService`
- `FollowService`

Services depend on repository abstractions rather than concrete repository implementations, which keeps business logic separated from MySQL-specific code and makes the services unit-testable.

### Repository Layer

Responsible for persistence and database queries.

Repository interfaces:

- `IUserRepository`
- `IPostRepository`
- `ICommentRepository`
- `ILikeRepository`
- `IFollowRequestRepository`
- `IFollowRepository`

Concrete repositories implement those interfaces using MySQL Connector/C++.

### Database Layer

- `Database` owns and provides access to the MySQL session.
- `TransactionManager` provides transaction handling for operations that need multiple database changes to succeed or fail together.

### Utilities

Reusable supporting functionality is kept under `src/utils`, including:

- Date/time conversion and formatting
- Follow-request status conversion
- Logging
- Menu input handling
- Password hashing
- User row mapping

## Project Structure

```text
social-network-backend/
├── src/
│   ├── application/
│   │   ├── menu/
│   │   │   ├── CommentMenu.cpp
│   │   │   ├── CommentMenu.hpp
│   │   │   ├── ConnectionMenu.cpp
│   │   │   ├── ConnectionMenu.hpp
│   │   │   ├── FollowMenu.cpp
│   │   │   ├── FollowMenu.hpp
│   │   │   ├── LikeMenu.cpp
│   │   │   ├── LikeMenu.hpp
│   │   │   ├── Menu.cpp
│   │   │   ├── Menu.hpp
│   │   │   ├── PostMenu.cpp
│   │   │   └── PostMenu.hpp
│   │   ├── Application.cpp
│   │   ├── Application.hpp
│   │   ├── LoggedInMenu.cpp
│   │   ├── LoggedInMenu.hpp
│   │   ├── LoggedOutMenu.cpp
│   │   └── LoggedOutMenu.hpp
│   │
│   ├── database/
│   │   ├── Database.cpp
│   │   ├── Database.hpp
│   │   ├── TransactionManager.cpp
│   │   └── TransactionManager.hpp
│   │
│   ├── models/
│   │   ├── Comment.hpp
│   │   ├── Follow.hpp
│   │   ├── FollowRequest.hpp
│   │   ├── FollowRequestStatus.hpp
│   │   ├── Like.hpp
│   │   ├── Post.hpp
│   │   └── User.hpp
│   │
│   ├── repositories/
│   │   ├── ICommentRepository.hpp
│   │   ├── IFollowRepository.hpp
│   │   ├── IFollowRequestRepository.hpp
│   │   ├── ILikeRepository.hpp
│   │   ├── IPostRepository.hpp
│   │   ├── IUserRepository.hpp
│   │   ├── CommentRepository.cpp/.hpp
│   │   ├── FollowRepository.cpp/.hpp
│   │   ├── FollowRequestRepository.cpp/.hpp
│   │   ├── LikeRepository.cpp/.hpp
│   │   ├── PostRepository.cpp/.hpp
│   │   └── UserRepository.cpp/.hpp
│   │
│   ├── services/
│   │   ├── CommentService.cpp/.hpp
│   │   ├── FollowService.cpp/.hpp
│   │   ├── LikeService.cpp/.hpp
│   │   ├── PostService.cpp/.hpp
│   │   └── UserService.cpp/.hpp
│   │
│   ├── utils/
│   │   ├── DateTimeUtils.cpp/.hpp
│   │   ├── FollowRequestStatusUtils.cpp/.hpp
│   │   ├── Logger.cpp/.hpp
│   │   ├── MenuInputUtils.cpp/.hpp
│   │   ├── PasswordHasher.cpp/.hpp
│   │   └── UserMapper.cpp/.hpp
│   │
│   └── main.cpp
│
├── tests/
│   ├── mocks/
│   │   ├── MockCommentRepository.hpp
│   │   ├── MockFollowRepository.hpp
│   │   ├── MockFollowRequestRepository.hpp
│   │   ├── MockLikeRepository.hpp
│   │   ├── MockPostRepository.hpp
│   │   ├── MockTransactionManager.hpp
│   │   └── MockUserRepository.hpp
│   │
│   ├── services/
│   │   ├── CommentServiceTest.cpp
│   │   ├── FollowServiceTest.cpp
│   │   ├── LikeServiceTest.cpp
│   │   ├── PostServiceTest.cpp
│   │   └── UserServiceTest.cpp
│   │
│   └── TestMain.cpp
│
├── CMakeLists.txt
├── CMakeSettings.json
├── schema.sql
├── README.md
└── .gitignore
```

## Database Design

The database contains six main tables:

| Table | Purpose |
|---|---|
| `users` | Stores user accounts and password hashes |
| `posts` | Stores posts created by users |
| `comments` | Stores comments associated with users and posts |
| `likes` | Represents likes between users and posts |
| `follow_requests` | Stores pending, accepted, or rejected follow requests |
| `follows` | Stores established follower/followee relationships |

Important relationships include:

- One user can create many posts.
- One user can create many comments.
- One post can contain many comments.
- Users and posts have a many-to-many relationship through `likes`.
- Users have self-referencing many-to-many relationships through `follow_requests` and `follows`.

Database constraints are also used to protect data integrity, including unique usernames and emails, duplicate-like prevention, and duplicate follow/follow-request prevention.

## Business Rules

The application enforces rules such as:

- Username and email must be unique.
- A user cannot follow themselves.
- Duplicate pending follow requests are not allowed.
- Duplicate likes are not allowed.
- Only the owner can update or delete their post.
- Only the owner can update or delete their comment.
- A follow relationship is created only after a follow request is accepted.
- Missing users, posts, comments, and other invalid operations produce clear errors.
- Access to another user's posts depends on the follow relationship.

## Technologies

- C++
- CMake
- MySQL 8
- MySQL Connector/C++
- libsodium — password hashing
- GoogleTest / GoogleMock — unit testing
- spdlog — application logging

## Configuration

Database credentials are read from environment variables rather than being hard-coded in the source code.

Set the following variables before running the application:

```text
DB_HOST
DB_PORT
DB_USER
DB_PASSWORD
DB_NAME
```

Example values:

```text
DB_HOST=localhost
DB_PORT=33060
DB_USER=root
DB_PASSWORD=your_password
DB_NAME=social_network
```

> `DB_PORT` must match the MySQL protocol/port used by your Connector/C++ configuration.

## Database Setup

Create the database schema using the included `schema.sql` file.

For example, load the script into your MySQL environment and execute it before starting the application.

The schema creates the tables and relationships required by the repository layer.

## Building

The project uses CMake. Make sure the required dependencies are installed and that your CMake configuration can locate them:

- MySQL Connector/C++
- libsodium
- spdlog
- GoogleTest / GoogleMock for the test target

Then configure and build the project using your CMake/IDE configuration.

On Windows, the project can be opened and built through Visual Studio's CMake support using the provided `CMakeLists.txt` and `CMakeSettings.json`.

## Running

Before launching the executable:

1. Make sure MySQL is running.
2. Create the database using `schema.sql`.
3. Set the required database environment variables.
4. Build the project.
5. Run the generated application executable.

The program starts in the logged-out menu, where a user can register or log in. After authentication, the logged-in menus provide access to posts, comments, likes, and follow-related operations.

## Logging

The project includes application logging with four levels:

- `debug`
- `info`
- `warn`
- `error`

Runtime logs are written to:

```text
out/build/x64-Debug/logs/social_network.log
```

when using the current Visual Studio debug build layout.

## Testing

The service layer is unit-tested with GoogleTest and GoogleMock.

Repository interfaces allow the real MySQL repositories to be replaced with mocks, so business rules can be tested independently from the database.

Current service test suites include:

- `UserServiceTest`
- `PostServiceTest`
- `CommentServiceTest`
- `LikeServiceTest`
- `FollowServiceTest`

This test structure focuses on service behaviour, validation, ownership/access rules, repository interactions, and error cases.

## Design Principles

The project applies several software-engineering practices:

- **Layered architecture** — separates presentation/application logic, business logic, and persistence.
- **Dependency inversion** — services depend on repository interfaces instead of MySQL repository implementations.
- **Single responsibility** — menus, services, repositories, database management, and utilities have distinct responsibilities.
- **RAII** — database resources are owned through C++ resource-management mechanisms.
- **Prepared/bound SQL parameters** — values are bound to SQL statements rather than concatenated into query strings.
- **Transactions** — multi-step database operations can be handled atomically.
- **Dependency injection** — repository abstractions are injected into services, enabling isolation and mocking during tests.
- **Centralised logging** — important application events and failures are recorded consistently.


