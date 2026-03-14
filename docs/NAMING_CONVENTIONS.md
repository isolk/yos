# Naming Conventions

## Current Issues

### Variable Naming Inconsistencies

1. **Page Table Variables:**
   - `paget_dir` (page.c, process.c) ❌
   - `page_dir` (process.h, process.c) ✅
   - **Should be:** `page_dir` (consistent with process.h)

2. **Allocation Functions:**
   - `ask_page()` / `ask_page_ptr()` (pm.h) ❌ Unclear naming
   - `kalloc()` / `kfree()` (pm.h) ✅ Clear kernel allocation
   - **Recommendation:** Use only `kalloc()`/`kfree()` for clarity

3. **Macro Definitions:**
   - `K` (1024) defined in multiple files
   - `K4` (4096) defined in page.c
   - **Should be:** Centralized in `memlayout.h` or `defs.h`

### Function Naming Patterns

**Good Patterns (to follow):**
- `init_<module>()` - Initialization functions
- `kalloc()` / `kfree()` - Kernel allocators
- `map_task()` / `cp_task_page_kernel()` - Clear action verbs
- `new_tss_kernel()` / `new_tss_user()` - Factory functions

**Inconsistent Patterns:**
- `mac()` - unclear function name (memory.c)
- `ask_page()` - unclear, should use `alloc_page()`

## Suggested Naming Conventions

### 1. Variable Naming
```c
// Use snake_case for variables and functions
page_dir          // Good
paget_dir         // Bad (typo)

kernel_base       // Good
KERNEL_BASE       // Good (for constants)
```

### 2. Function Naming
```c
// Use descriptive action verbs
kalloc()          // Good - kernel allocate
kfree()           // Good - kernel free
init_page_dir()   // Good - initialize page directory
map_task()        // Good - map task memory

mac()             // Bad - unclear
ask_page()        // Bad - use alloc_page() instead
```

### 3. Macro Naming
```c
// Use UPPER_CASE for macros
#define PAGE_SIZE 4096        // Good
#define KERNEL_BASE 0xC0000000u // Good

#define K 1024                // Acceptable but unclear
#define PAGE_SIZE 4096        // Better - more descriptive
```

### 4. Type Naming
```c
// Use snake_case for struct types when not typedef'd
struct page_table { ... }    // Good

// Use snake_case for typedef'd types
typedef struct task_struct task_struct;  // Good
typedef struct page_table page_table;    // Good
```

## Migration Priority

### High Priority (Functional Issues)
1. Fix `paget_dir` → `page_dir` (typo causing inconsistency)
2. Remove unused `ask_page()` / `ask_page_ptr()` functions
3. Consolidate `K` / `K4` macros into shared header

### Medium Priority (Clarity Improvements)
1. Rename `mac()` → `bitmap_alloc()` or implement properly
2. Standardize function prefixes (k* for kernel, u* for user)
3. Add consistent naming for page table operations

### Low Priority (Style)
1. Review all variable names for clarity
2. Ensure consistent use of typedef vs struct tags
3. Document non-obvious abbreviations

## Constants to Centralize

Create `kernel/include/defs.h` or add to `memlayout.h`:
```c
#define PAGE_SIZE       4096
#define PAGE_SHIFT      12
#define KERNEL_BASE     0xC0000000u
#define USER_END        (3 * 1024 * 1024 * 1024u)
#define MB              (1024 * 1024)
#define KB              1024
```

## Notes

- Naming consistency improves code readability
- Clear function names reduce need for comments
- Prefer descriptive names over abbreviations
- When in doubt, use longer, clearer names
