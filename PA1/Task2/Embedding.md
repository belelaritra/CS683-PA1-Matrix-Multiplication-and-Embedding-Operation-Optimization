# 🎬 Movie Embedding Bagging

## 1. 📊 Embedding Table (4 Dimensions)
Each movie is represented by a vector of 4 scores (dimensions):

- **Dim 1** → Action score  
- **Dim 2** → Comedy score  
- **Dim 3** → Romance score  
- **Dim 4** → Horror score  

| Index | Movie                  | Action | Comedy | Romance | Horror |
|-------|------------------------|--------|--------|---------|--------|
| 0     | Family Man (Web)       | **0.85**   | 0.25   | 0.10    | 0.05   |
| 1     | Avengers (English)     | **0.90**   | 0.35   | 0.10    | 0.00   |
| 2     | Inception (English)    | **0.80**   | 0.05   | 0.10    | 0.05   |
| 3     | Matrix (English)       | **0.90**   | 0.05   | 0.05    | 0.00   |
| 4     | Special Ops (Web)      | **0.92**   | 0.10   | 0.05    | 0.00   |
| 5     | Jab We Met (Hindi)     | 0.10   | **0.40**   | **1.00**    | 0.00   |
| 6     | Student of the Year    | 0.15   | **0.60**   | **0.80**    | 0.00   |
| 7     | DDLJ (Hindi)           | 0.10   | **0.30**   | **1.00**    | 0.00   |
| 8     | 3 Idiots (Hindi)       | 0.10   | **1.00**   | 0.35    | 0.00   |
| 9     | Stree (Horror Comedy)  | 0.20   | **0.65**   | 0.20    | **0.80**   |
| 10    | Bhool Bhulaiyaa        | 0.18   | **0.55**   | 0.15    | **0.85**   |

---

## 2. 🎭 Categorization
- 🔥 **Action**: Family Man, Avengers, Inception, Matrix, Special Ops  
- 💕 **Romance**: Jab We Met, Student of the Year, DDLJ  
- 😂 **Comedy**: 3 Idiots, Stree, Bhool Bhulaiyaa  
- 👻 **Horror**: Stree, Bhool Bhulaiyaa  

---

## 3. 👥 Users & Watched Movies

- 👤 **User Aritra** (Likes Action & Thriller)  
  Watched: 
  - Family Man (0), 
  - Avengers (1), 
  - Inception (2), 
  - Matrix (3), 
  - Special Ops (4)  

- 👤 **User Sougata** (Likes Romance & RomCom)  
  Watched: 
    - Jab We Met (5), 
    - Student of the Year (6), 
    - DDLJ (7)  

- 👤 **User Hariom** (Likes Comedy & Horror Comedy)  
  Watched: 
  - 3 Idiots (8), 
  - Stree (9), 
  - Bhool Bhulaiyaa (10)  

---

## 4. 📝 Input Representation
- **Input vector** (all movies by users):  
  `[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]`  
- **Offsets** (start index of each user's bag):  
  `[0, 5, 8]`  

**Meaning**:  
- User **Aritra** → indices 0 to 4  
- User **Sougata** → indices 5 to 7  
- User **Hariom** → indices 8 to 10  

---

## 5. 🧮 Bagging Calculation
## 📊 User-wise Embedding Sum Calculation

### 👤 User Aritra (Action Lover)

| Movie             | Action | Comedy | Romance | Horror |
|-------------------|--------|--------|---------|--------|
| Family Man        | 0.85   | 0.25   | 0.10    | 0.05   |
| Avengers          | 0.90   | 0.35   | 0.10    | 0.00   |
| Inception         | 0.80   | 0.05   | 0.10    | 0.05   |
| Matrix            | 0.90   | 0.05   | 0.05    | 0.00   |
| Special Ops       | 0.92   | 0.10   | 0.05    | 0.00   |
| **Sum**           | **4.37** | **0.80** | **0.40**  | **0.10** |
---
**Final Vector**  : (**4.37**, 0.80, 0.40, 0.10) 



### 👤 User Sougata (Romance Lover)

| Movie                | Action | Comedy | Romance | Horror |
|----------------------|--------|--------|---------|--------|
| Jab We Met           | 0.10   | 0.40   | 1.00    | 0.00   |
| Student of the Year  | 0.15   | 0.60   | 0.80    | 0.00   |
| DDLJ                 | 0.10   | 0.30   | 1.00    | 0.00   |
| **Sum**              | **0.35** | **1.30** | **2.80**  | **0.00** |
---
**Final Vector**  :   (0.35, 1.30, **2.80**, 0.00)



### 👤 User Hariom (Comedy + Horror Lover)

| Movie           | Action | Comedy | Romance | Horror |
|-----------------|--------|--------|---------|--------|
| 3 Idiots        | 0.10   | 1.00   | 0.35    | 0.00   |
| Stree           | 0.20   | 0.65   | 0.20    | 0.80   |
| Bhool Bhulaiyaa | 0.18   | 0.55   | 0.15    | 0.85   |
| **Sum**         | **0.48** | **2.20** | **0.70**  | **1.65** |
---

**Final Vector** : (0.48, **2.20**, 0.70, **1.65**) 

<!-- 
### 👤 User Aritra (Action Lover)
**Movies watched**:  
Family Man (0.85, 0.25, 0.10, 0.05)  
Avengers (0.90, 0.35, 0.10, 0.00)  
Inception (0.80, 0.05, 0.10, 0.05)  
Matrix (0.90, 0.05, 0.05, 0.00)  
Special Ops (0.92, 0.10, 0.05, 0.00)  

**Sum**:  
- Action: 4.37  
- Comedy: 0.80  
- Romance: 0.40  
- Horror: 0.10  

✅ **Result**: `(4.37, 0.80, 0.40, 0.10)`

---

### 👤 User Sougata (Romance Lover)
**Movies watched**:  
Jab We Met (0.10, 0.40, 1.00, 0.00)  
Student of the Year (0.15, 0.60, 0.80, 0.00)  
DDLJ (0.10, 0.30, 1.00, 0.00)  

**Sum**:  
- Action: 0.35  
- Comedy: 1.30  
- Romance: 2.80  
- Horror: 0.00  

✅ **Result**: `(0.35, 1.30, 2.80, 0.00)`

---

### 👤 User Hariom (Comedy + Horror Lover)
**Movies watched**:  
3 Idiots (0.10, 1.00, 0.35, 0.00)  
Stree (0.20, 0.65, 0.20, 0.80)  
Bhool Bhulaiyaa (0.18, 0.55, 0.15, 0.85)  

**Sum**:  
- Action: 0.48  
- Comedy: 2.20  
- Romance: 0.70  
- Horror: 1.65  

✅ **Result**: `(0.48, 2.20, 0.70, 1.65)` -->

---

## 6. 📦 Output

| User    | Final Embedding Vector     |
|---------|----------------------------|
| Aritra  | (**4.37**, 0.80, 0.40, 0.10)   |
| Sougata | (0.35, 1.30, **2.80**, 0.00)   |
| Hariom  | (0.48, **2.20**, 0.70, **1.65**)   |

---

## 7. 💻 Pseudocode
```bash
START Embedding Bagging:

FOR 'EACH user': 
    Create a bag with 4 empty slots [0, 0, 0, 0]

'FOR EACH movie' the user watched: 
    'Get the movie`s scores' from the embedding table 
    'Add each score' to the corresponding slot in the bag

'Save the final bag' as the user`s profile

END
```

**In simple terms**:  
1. Take an empty bag for each user  
2. For every movie they watched, grab its genre scores  
3. Keep adding those scores to the bag  
4. The final bag shows what the user likes most  

---

## 8. 🎯 Interpretation & Recommendations

- 🔥 **User Aritra** (Action Score: 4.37)  
  *Action enthusiast*  
  **Recommend**: Mission Impossible, Extraction, War, Fast & Furious  

- 💕 **User Sougata** (Romance Score: 2.80)  
  *Romance and emotional storytelling lover*  
  **Recommend**: Kal Ho Naa Ho, Yeh Jawaani Hai Deewani  

- 😂👻 **User Hariom** (Comedy: 2.20, Horror: 1.65)  
  *Enjoys horror-comedy blend*  
  **Recommend**: Go Goa Gone, Roohi, Housefull series  

---

## 9. 🚀 Why This Matters
This embedding bagging operation is the **core of recommendation systems**:

- Netflix → understands your viewing patterns  
- Amazon → personalizes product suggestions  
- Spotify → tailors your music discovery  

By **aggregating interactions**, AI systems learn your preferences and suggest similar content!
