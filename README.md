# R.E.P.O. Save Backup & Restore Tool

Had a difficult level and lost a long run? This tool will help you to **backup and restore game save files** from the folder:

```
C:\Users\<Username>\AppData\LocalLow\semiwork\Repo\saves
```

## 🔧 Features

- ♻️ Full restore from backup
- 📁 Uses Windows API (Win32) for native file operations
- 🤖 Detects current Windows user automatically

## 📦 Folder Structure

- **saves**: your original save data
- **saves_bkp**: where backups are stored

## 🚀 How to Use

1. Run the executable
2. Choose:
   - `1` to backup saves
   - `2` to restore from backup (confirmation required)

> The tool will show error messages if folders don't exist or operations fail.

## ⚠️ Warning

Restoring **overwrites and deletes** the current `saves` folder. Always confirm before restoring.

There is use of "offensive" language in some of the errors, so beware if you "test" the program's patience, ok? 
(No intention of actually offending someone, just a joke, ok?)

Feel free to report bugs or improvements via GitHub.
