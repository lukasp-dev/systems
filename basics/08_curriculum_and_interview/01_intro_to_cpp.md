### Declaration: The symbolic metadata

A **Declaration** tells the compiler that there exists a name with this specific type.

```cpp
int add(int a, int b);   // declaration only
extern int global_count; // declaration only
```

```cpp
int add(int a, int b) { return a + b; } // definition (also a declaration)
int global_count = 0;                   // definition
```

---

### Curriculum order (basics first)
1. `..\01_core_grammar_and_oop\`
2. `..\02_value_categories\`
3. `..\03_auto_and_type_deduction\`
4. `..\04_size_t_and_indexing\`
5. `..\05_raii_and_pointer_pitfalls\`
6. `..\06_pointers_deep_dive\` (advanced: includes `10_modern_views_packet_demo.md`)
7. `..\07_templates\`
8. then this folder `08_curriculum_and_interview\` for interview-focused review

### Interview folder order (after fundamentals)
1. `02_stl_and_complexity.md`
2. `03_modern_value_types.md`
3. `04_coding_problem_workflow.md`
4. `05_quick_sheet_pitfalls.md`
