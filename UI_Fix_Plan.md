# UI Fix Plan - BhashaQuest Lesson View

**Date:** November 10, 2025
**Objective:** Fix critical UI/UX issues identified in the LessonView screenshot

---

## Context: Initial Application Load

This screenshot captures the **first view users see when launching the application** after selecting a skill (Food & Dining). This is a critical user experience moment and sets the impression for the entire application.

**PRIMARY ISSUE: Readability** 🔴
The main problem is **poor visibility and readability of lesson content**. Text is faint, invisible, or obscured, making it impossible for users to read the exercise instructions and engage with the lesson material.

---

## Issues Identified

### 1. **Exercise Counter Logic Error** ❌
**Severity:** High
**Issue:** "Exercise 1 of 0" displayed at the top
- Shows exercise 1 but indicates 0 total exercises (illogical state)
- Suggests content loading issue or incorrect exercise count initialization

**Root Cause:** Likely the exercise list is empty or not properly loaded/initialized

**Fix Required:**
- Verify exercise data is properly loaded from content.json
- Ensure exercise count is calculated correctly before display
- Show proper counter only when exercises are available
- Display loading state or message if no exercises exist

---

### 2. **Prominent Watermark Obstruction** ❌
**Severity:** High
**Issue:** Large "BhashaQuest" watermark dominates the center of the lesson view
- Obscures actual lesson content
- Makes the application look unpolished
- Impairs usability and readability

**Root Cause:** Watermark/logo rendering on top of lesson content without proper layering

**Fix Required:**
- Remove or significantly reduce watermark opacity
- Reposition watermark to a corner (bottom-right suggested) if branding is needed
- Make watermark background-only (very subtle, low opacity)
- Alternatively: move to login/splash screen only

---

### 3. **Missing/Invisible Lesson Content** ❌
**Severity:** High
**Issue:** Lesson exercise area appears mostly empty with faint/invisible text
- Main content area lacks visible instruction text
- Questions/prompts not clearly readable
- Appears to be rendering issues or color contrast problems

**Root Cause:**
- Text color may be too light (white on white or similar)
- Content may not be properly rendered from JSON
- Font sizes may be too small

**Fix Required:**
- Verify text color contrast (should be dark text on light background)
- Ensure exercise content is properly loaded and displayed
- Check font sizes are appropriate and readable
- Add debugging to verify content data is present

---

### 4. **Icon Array at Bottom** ⚠️
**Severity:** Medium
**Issue:** Row of icons (star ⭐, target 🎯, bar chart 📊, graph 📈) at bottom lacks context
- Icons appear to be placeholder or debug elements
- No clear purpose or labels
- Takes up space in lesson area

**Root Cause:** UI elements may be leftover from development/debugging

**Fix Required:**
- Determine intended purpose of icons
- If not needed: remove these elements
- If needed: add labels and proper styling
- Position icons appropriately (currently floating without context)

---

### 5. **Large Blue Rectangle Element** ❌
**Severity:** Medium
**Issue:** Large blue box at bottom of lesson area
- No visible content or purpose
- Appears to be a placeholder or rendering error
- Takes up significant space

**Root Cause:**
- May be a button placeholder
- Could be uninitialized widget/layout
- Possible styling/rendering error

**Fix Required:**
- Identify what this element should be
- Implement proper content/styling
- Or remove if not needed
- Ensure all interactive elements are clearly labeled

---

### 6. **Layout & Spacing Issues** ⚠️
**Severity:** Medium
**Issue:** Uneven spacing and alignment in lesson view
- Content distribution appears unbalanced
- Large empty spaces between elements
- Navigation area (buttons, selector) positioning could be improved

**Fix Required:**
- Review layout hierarchy
- Implement proper spacing/margins
- Ensure responsive layout
- Align elements consistently

---

## Priority Fixes

### Phase 1 (Critical - Must Fix)
1. Fix "Exercise 1 of 0" counter logic
2. Remove or hide watermark
3. Fix invisible/unreadable lesson content
4. Properly render exercise data

### Phase 2 (Important)
5. Clarify/fix blue rectangle element
6. Remove or contextualize icon row
7. Improve overall layout spacing

---

## Files to Review/Modify

- **LessonView.h** - Widget structure and data members
- **LessonView.cpp** - Content rendering, layout, styling
- **mainwindow.cpp** - Integration with main window
- **content.json** - Verify exercise data structure
- **CSS/Stylesheets** - Check colors, fonts, contrast

---

## Testing Checklist

After fixes are implemented:

- [ ] Exercise counter shows correct count (e.g., "Exercise 1 of 5")
- [ ] Watermark removed or minimally visible
- [ ] Lesson content (text, questions) clearly readable
- [ ] All UI elements have clear purpose or are removed
- [ ] Layout looks balanced and professional
- [ ] Text contrast meets accessibility standards
- [ ] No placeholder/debug elements visible
- [ ] Responsive design works on different window sizes
- [ ] Audio buttons (if any) function properly

---

## Thematic Color Scheme Recommendations

To improve readability and create a cohesive, professional design for a **language learning application**, consider adopting a thematic color palette:

### Suggested Scheme: **Warm & Educational**
- **Primary Background:** Light cream/off-white (#F8F6F1 or #FAFAF8) - reduces eye strain
- **Content Text:** Dark charcoal/navy (#2C3E50 or #1A1A1A) - high contrast for readability
- **Accent Color:** Warm teal/turquoise (#1ABC9C or #16A085) - energetic, learning-focused
- **Secondary Accent:** Warm orange/coral (#E67E22) - warmth and engagement
- **Buttons:**
  - Green for positive actions: #27AE60 (Start Lesson) ✓
  - Blue for info: #3498DB (View Profile) ✓
- **Watermark/Background Elements:** Very subtle gray (#E8E8E8) at 5-10% opacity
- **Lesson Area Background:** Pure white (#FFFFFF) with subtle shadow border

### Text Hierarchy
- **Exercise Counter:** Bold, dark color (#2C3E50)
- **Exercise Instructions:** Regular weight, dark color, minimum 14-16px font
- **Exercise Questions:** Bold, dark color, 14-16px or larger
- **Help Text/Labels:** Medium gray (#666666), slightly smaller

### Accessibility Considerations
- Ensure WCAG AA compliance (4.5:1 contrast ratio for body text)
- Use color + icons for status indication (don't rely on color alone)
- Test readability on different screen brightness levels

---

## Notes

- The application appears to have content structure in place (buttons work, skill selector works)
- The issue is primarily with rendering/displaying the actual lesson content
- Data loading seems to be the core problem (empty exercise count suggests data isn't loaded)
- **CRITICAL: Fix readability first** - everything else is secondary to users being able to read the lesson
- Consider implementing a light/dark theme toggle for accessibility
- Focus on content visibility and data validation as the highest priority
