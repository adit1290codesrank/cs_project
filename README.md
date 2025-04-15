# 💸 Cash Flow Minimizer

A fun and efficient way to manage group expenses and settle debts with the **least number of transactions**!

---

## 📌 Overview

Managing shared expenses with friends (rent, trips, outings, etc.) can get messy. This project simplifies it by letting users:
- Create groups
- Add transactions
- Automatically minimize the number of payments needed to settle up

Think of it as the smartest way to "settle up" – no more ₹2 back-and-forth 5 times 😅

---

## 🧠 How It Works

We use a **graph-based algorithm** where:
- Each user is a **node**
- Debts are **directed weighted edges**
- The app calculates the **minimum cash flow** needed to settle all transactions

### 📉 Optimization:
Max possible transactions between _n_ users: `n(n-1)/2`  
After optimization: just `n - 1`

---

## 🚀 Features

- 👥 **Group Creation** – Create groups for trips, roommates, anything!
- ✍️ **Add Transactions** – Who paid what, for whom, how much
- 📊 **Live Balances** – See who owes whom in real-time
- 🔁 **Cash Flow Minimization** – Fewer transactions, faster settlements
- 🧾 **Settle Up Suggestions** – Clear and simple payment instructions

---

## 🛠️ Tech Stack

- **Frontend:** HTML, CSS, Bootstrap5
- **Backend:** C++(Crow)
- **Database:** MySQL
- **Algorithm:** Graph theory (Directed, Weighted Graphs) + Heaps for optimization

---
