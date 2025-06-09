#include "io.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// UTF8

// NOTE: The function also advances the pointer to the next character
static int decodeUtf8(const char **str) {
  const unsigned char *s = (const unsigned char *)*str;
  int codepoint = 0;

  if (*s < 0x80) {
    // 1-byte character: 0xxxxxxx
    codepoint = *s++;
  } else if ((*s & 0xE0) == 0xC0) {
    // 2-byte character: 110xxxxx 10xxxxxx
    codepoint = (*s++ & 0x1F) << 6;
    codepoint |= (*s++ & 0x3F);
  } else if ((*s & 0xF0) == 0xE0) {
    // 3-byte character: 1110xxxx 10xxxxxx 10xxxxxx
    codepoint = (*s++ & 0x0F) << 12;
    codepoint |= (*s++ & 0x3F) << 6;
    codepoint |= (*s++ & 0x3F);
  } else if ((*s & 0xF8) == 0xF0) {
    // 4-byte character: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    codepoint = (*s++ & 0x07) << 18;
    codepoint |= (*s++ & 0x3F) << 12;
    codepoint |= (*s++ & 0x3F) << 6;
    codepoint |= (*s++ & 0x3F);
  } else {
    // Invalid UTF-8
    return -1;
  }

  *str = (const char *)s;
  return codepoint;
}

// Checks string
bool isValidUtf8(const char *str) {
  if (!str)
    return false;

  const char *ptr = str;

  while (*ptr) {
    int codepoint = decodeUtf8(&ptr);
    if (codepoint == -1) {
      handleError("Invalid UTF-8 sequence.");
      return false;
    }
  }

  return true;
}

size_t getUtf8StringLength(const char *str) {
  const char *ptr = str;
  size_t length = 0;

  while (*ptr) {
    decodeUtf8(&ptr);
    length++;
  }

  return length;
}

static char *readUtf8Char(const char *source) {
  const char *ptr = source;
  int codepoint = decodeUtf8(&ptr);

  if (codepoint == -1) {
    return NULL; // Invalid UTF-8
  }

  size_t len = ptr - source;
  char *buffer = malloc(len + 1); // +1 for null terminator
  if (!buffer) {
    return NULL;
  }

  memcpy(buffer, source, len);
  buffer[len] = '\0';
  return buffer;
}

size_t findCharsInUtf8String(const char *str, const char *reject) {
  const char *strPtr = str;
  size_t pos = 0;

  while (*strPtr) {
    const char *rejectPtr = reject;
    int codepointStr = decodeUtf8(&strPtr);

    if (codepointStr == -1) {
      handleError("Invalid UTF-8 character encountered.");
      return pos;
    }

    while (*rejectPtr) {
      int codepointReject = decodeUtf8(&rejectPtr);

      if (codepointReject == -1) {
        handleError("Invalid UTF-8 character in reject set.");
        return pos;
      }

      if (codepointStr == codepointReject) {
        return pos;
      }
    }

    pos++;
  }

  return pos;
}

int compareUtf8Strings(const char *str1, const char *str2) {
  while (*str1 && *str2) {
    int codepoint1 = decodeUtf8(&str1);
    int codepoint2 = decodeUtf8(&str2);

    if (codepoint1 < 0 || codepoint2 < 0) {
      // Handle invalid UTF-8
      return codepoint1 - codepoint2;
    }

    if (codepoint1 != codepoint2) {
      return codepoint1 - codepoint2;
    }
  }

  // If one string is longer, it's greater
  return *str1 - *str2;
}

bool hasSuffix(const char *word, const char *suffix) {
  size_t wordLength = getUtf8StringLength(word);
  size_t suffixLength = getUtf8StringLength(suffix);

  if (wordLength < suffixLength) {
    return false;
  }

  const char *wordPtr = word;
  const char *suffixPtr = suffix;

  // Move to the possible position for the suffix
  for (int i = 0; i < wordLength - suffixLength; i++)
    decodeUtf8(&wordPtr);

  while (*suffixPtr) {
    int wordCodepoint = decodeUtf8(&wordPtr);
    int suffixCodepoint = decodeUtf8(&suffixPtr);

    if (wordCodepoint != suffixCodepoint)
      return false;
  }

  return true;
}

bool isWhitespace(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
          c == '\f');
}

bool isLineEmpty(const char *line) {
  if (!line)
    return true;

  for (size_t i = 0; line[i] != '\0'; i++) {
    if (!isWhitespace(line[i]) && line[i] != ' ') {
      return false;
    }
  }
  return true;
}

char *trimWhitespaceUtf8(char *str) {
  if (!str) {
    handleError("Passed NULL to trimWhitespaceUtf8");
    return NULL;
  }

  char *start = str;
  char *end = str + strlen(str);

  // trim initial
  while (*start && isWhitespace(*start)) {
    start++;
  }

  // trim trailing
  while (end > start && isWhitespace(*(end - 1))) {
    end--;
  }

  *end = '\0';

  size_t trimmedLength = strlen(start);
  memmove(str, start, trimmedLength + 1);

  return str;
}

char *readStringWithFilterUntilValid(const char *prompt, size_t maxLength,
                                     const char *reject) {
  char *input = NULL;
  bool isReadingInput = true;

  do {
    printf("%s: ", prompt);
    int result = readLine(&input, DEFAULT_STRING_LENGTH, maxLength, stdin);

    if (result == FAILURE)
      return NULL;

    if (result == EOF)
      return NULL;

    if (!isValidUtf8(input)) {
      free(input);
      input = NULL;
      continue;
    }

    size_t initialLength = getUtf8StringLength(input);
    trimWhitespaceUtf8(input);

    isReadingInput = false;
  } while (isReadingInput);

  return input;
}
