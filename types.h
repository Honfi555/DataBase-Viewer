#ifndef TYPES_H
#define TYPES_H

enum QSearchType {
    Simple,
    Complicated
};

enum QConnectionType {
    ORM,
    Connected
};

enum QFileType {
    csv,
    txt
};

enum QSqlRights {
    Select,
    Insert,
    // Update,
    Delete,
    Usage
};

#endif // TYPES_H
