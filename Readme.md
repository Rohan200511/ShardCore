# 🛡️ ShardCore – Distributed Key-Value Store in C++

ShardCore is a distributed key-value storage system built in C++ that simulates core concepts of modern backend systems such as Redis, DynamoDB, and Cassandra.

It demonstrates how data can be distributed, replicated, and persisted across multiple nodes using system design principles.

---

## 🧠 Overview

ShardCore is designed as a multi-node system where each node runs on a separate port and stores data independently. A client communicates with these nodes using TCP sockets, and data is distributed using consistent hashing.

---

## ⚙️ Features

- 🔹 Multi-node architecture (runs on multiple ports)
- 🔹 Consistent hashing for efficient data distribution
- 🔹 Replication for fault tolerance
- 🔹 Append-only log (AOF) for persistence
- 🔹 Fast in-memory storage using unordered_map

---
## 🧠 Consistent Hashing Explained

Consistent hashing is used to distribute keys across nodes efficiently with minimal data movement when nodes are added or removed.

📖 Read detailed explanation:
👉 https://www.geeksforgeeks.org/system-design/consistent-hashing/

---
## 🏗️ Architecture


        Client → Hash Ring → Node (Server) → KV Store → Disk (AOF)


### Components:
- **Client**: Sends PUT, GET, DELETE requests
- **HashRing**: Determines which node stores the key
- **Server Nodes**: Handle requests and store data
- **KVStore**: In-memory key-value storage
- **Persistence Layer**: Append-only log files

---

## 🔁 How It Works

### PUT Operation
1. Key is hashed using consistent hashing
2. Primary node is selected
3. Data is stored in primary node
4. Data is replicated to next node (replica)
5. Operation is logged to disk

---

### GET Operation
1. Key is hashed
2. Request goes to primary node
3. If primary fails → fallback to replica

---

### DELETE Operation
- Removes key from both primary and replica

---

## 🧠 Consistent Hashing

Instead of using `hash(key) % N`, ShardCore uses a hash ring:

- Nodes are placed on a circular hash space
- Keys are mapped to the nearest node clockwise
- Minimizes data movement when nodes are added/removed

---

## 💾 Persistence (AOF)

Each node maintains its own log file:


        data.log


- All write operations are appended to file
- On restart, data is restored by replaying logs

---

## 🧪 Demo

### 1. Start servers

        shardcore 9090
        
        shardcore 9091
        
        shardcore 9092


### 2. Run client

        client


### 3. Example commands

        PUT user1 Rohan
        
        GET user1
        
        DELETE user1


---

## ⚡ Fault Tolerance

- Data is replicated across nodes
- If one node fails, replica serves the request
- Ensures high availability

---

## 📦 Tech Stack

- C++
- Winsock (TCP networking)
- STL (unordered_map, map, vector)
- File handling (fstream)

---

## 🎯 Use Cases

- Caching layer (like Redis)
- Session storage
- High-speed key-value access systems
- Distributed backend systems

---

## 📚 What I Learned

- Distributed system design fundamentals
- Consistent hashing vs modulo hashing
- Fault tolerance using replication
- Persistence using append-only logs
- TCP-based client-server communication

---

## 🏆 Future Improvements

- Thread pool for concurrent handling
- Virtual nodes for better load balancing
- Leader election
- Monitoring dashboard

---

## 🤝 Contributing

Feel free to fork and improve the project!

---

## ⭐ If you like this project, give it a star!
