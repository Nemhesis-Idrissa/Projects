# Projects

## Overview

This C++ application, built using the Qt framework, provides a simple interface for translating text between English and French. It leverages dictionaries stored in text files to perform translations and includes functionality for correcting input text using spell-checking techniques.

**Note:** This project was completed within two weeks as part of an English language assignment.

## Features

- **Translation:** Allows users to enter a sentence in either English or French and receive its translation in the opposite language.
- **Correction:** Includes a spell-checking feature that suggests corrections for input text based on a pre-defined dictionary.
- **User Interface:** Provides a clean and intuitive user interface implemented using Qt's QMainWindow and various widgets.

## Components

The application consists of:

- **TranslatorWindow Class:** Handles the main window interface, including text input, translation buttons, and display areas for original and translated text.
- **Translation Logic:** Uses hash tables (`QHash`) to store mappings between English and French phrases for efficient lookups during translation.
- **File Handling:** Reads dictionaries from external text files to populate translation tables, ensuring easy updates and expansions of language resources.
- **Efficiency:** Utilizes Qt's signal-slot mechanism for handling user interactions.

## Requirements

- **Qt Framework:** Version 6.7 or higher.
- **C++ Compiler:** Compatible with C++17 standard.

## Usage

1. **Compile the Application:** Use CMake to generate build files and compile the application.
2. **Run the Application:** Execute the compiled executable to launch the translator interface.
3. **Enter Text:** Input sentences in either English or French into the designated text areas.
4. **Translate and Correct:** Click the "Translate" or "Traduire" button to see the corresponding translation along with any suggested corrections.

![Translator Application](https://github.com/Nemhesis-Idrissa/Projects/assets/43876545/5485f442-08ae-4884-b887-204459c9c75e)





# Boyer-Moore Search with QtConcurrent

This project implements a multithreaded text search using the Boyer-Moore algorithm with QtConcurrent. The program reads a large text file, splits it into chunks, and processes each chunk in parallel to search for a specified expression. The objectives were to enhance the search mechanism in the translator app, addressing inefficiencies when processing large files such as .csv or text files with thousands of rows.
## Features

- **Multithreaded Processing**: Uses `QtConcurrent::run` to process chunks of the file in parallel.
- **Efficient Search**: Implements the Boyer-Moore algorithm for efficient string searching.
- **Asynchronous Result Handling**: Utilizes `QFutureWatcher` to monitor and handle the results of each asynchronous computation.
- **Configurable Chunk Size**: Allows adjustment of chunk size for performance optimization.

## Requirements

- Qt 5.15 or later
- C++17 or later

## Results 
![image](https://github.com/Nemhesis-Idrissa/Projects/assets/43876545/26e33ff7-8622-4ae0-b6c8-064a62646238)
