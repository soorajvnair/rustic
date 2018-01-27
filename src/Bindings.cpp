
#include "Bindings.h"

namespace {

    struct option_quintptr {
    public:
        quintptr value;
        bool some;
        operator QVariant() const {
            if (some) {
                return QVariant(value);
            }
            return QVariant();
        }
    };

    struct option_QString {
    public:
        QString value;
        bool some;
        operator QVariant() const {
            if (some) {
                return QVariant(value);
            }
            return QVariant();
        }
    };

    struct option_quint64 {
    public:
        quint64 value;
        bool some;
        operator QVariant() const {
            if (some) {
                return QVariant(value);
            }
            return QVariant();
        }
    };

    struct qstring_t {
    private:
        const void* data;
        int len;
    public:
        qstring_t(const QString& v):
            data(static_cast<const void*>(v.utf16())),
            len(v.size()) {
        }
        operator QString() const {
            return QString::fromUtf8(static_cast<const char*>(data), len);
        }
    };
    typedef void (*qstring_set)(QString*, qstring_t*);
    void set_qstring(QString* v, qstring_t* val) {
        *v = *val;
    }

    struct qbytearray_t {
    private:
        const char* data;
        int len;
    public:
        qbytearray_t(const QByteArray& v):
            data(v.data()),
            len(v.size()) {
        }
        operator QByteArray() const {
            return QByteArray(data, len);
        }
    };
    typedef void (*qbytearray_set)(QByteArray*, qbytearray_t*);
    void set_qbytearray(QByteArray* v, qbytearray_t* val) {
        *v = *val;
    }

    struct qmodelindex_t {
        int row;
        quintptr id;
    };
    inline void fibonacciInputChanged(Fibonacci* o)
    {
        emit o->inputChanged();
    }
    inline void fibonacciResultChanged(Fibonacci* o)
    {
        emit o->resultChanged();
    }
    inline void fileSystemTreePathChanged(FileSystemTree* o)
    {
        emit o->pathChanged();
    }
    inline void processesActiveChanged(Processes* o)
    {
        emit o->activeChanged();
    }
}
extern "C" {
    Demo::Private* demo_new(Demo*, Fibonacci*, void (*)(Fibonacci*), void (*)(Fibonacci*), FibonacciList*,
        void (*)(const FibonacciList*),
        void (*)(FibonacciList*, quintptr, quintptr),
        void (*)(FibonacciList*),
        void (*)(FibonacciList*),
        void (*)(FibonacciList*, int, int),
        void (*)(FibonacciList*),
        void (*)(FibonacciList*, int, int),
        void (*)(FibonacciList*), FileSystemTree*, void (*)(FileSystemTree*),
        void (*)(const FileSystemTree*, quintptr, bool),
        void (*)(FileSystemTree*, quintptr, quintptr),
        void (*)(FileSystemTree*),
        void (*)(FileSystemTree*),
        void (*)(FileSystemTree*, option_quintptr, int, int),
        void (*)(FileSystemTree*),
        void (*)(FileSystemTree*, option_quintptr, int, int),
        void (*)(FileSystemTree*), Processes*, void (*)(Processes*),
        void (*)(const Processes*, quintptr, bool),
        void (*)(Processes*, quintptr, quintptr),
        void (*)(Processes*),
        void (*)(Processes*),
        void (*)(Processes*, option_quintptr, int, int),
        void (*)(Processes*),
        void (*)(Processes*, option_quintptr, int, int),
        void (*)(Processes*), TimeSeries*,
        void (*)(const TimeSeries*),
        void (*)(TimeSeries*, quintptr, quintptr),
        void (*)(TimeSeries*),
        void (*)(TimeSeries*),
        void (*)(TimeSeries*, int, int),
        void (*)(TimeSeries*),
        void (*)(TimeSeries*, int, int),
        void (*)(TimeSeries*));
    void demo_free(Demo::Private*);
    Fibonacci::Private* demo_fibonacci_get(const Demo::Private*);
    FibonacciList::Private* demo_fibonacci_list_get(const Demo::Private*);
    FileSystemTree::Private* demo_file_system_tree_get(const Demo::Private*);
    Processes::Private* demo_processes_get(const Demo::Private*);
    TimeSeries::Private* demo_time_series_get(const Demo::Private*);
};

extern "C" {
    Fibonacci::Private* fibonacci_new(Fibonacci*, void (*)(Fibonacci*), void (*)(Fibonacci*));
    void fibonacci_free(Fibonacci::Private*);
    quint32 fibonacci_input_get(const Fibonacci::Private*);
    void fibonacci_input_set(Fibonacci::Private*, uint);
    quint64 fibonacci_result_get(const Fibonacci::Private*);
};

extern "C" {
    quint64 fibonacci_list_data_fibonacci_number(const FibonacciList::Private*, int);
    quint64 fibonacci_list_data_row(const FibonacciList::Private*, int);
    void fibonacci_list_sort(FibonacciList::Private*, unsigned char column, Qt::SortOrder order = Qt::AscendingOrder);

    int fibonacci_list_row_count(const FibonacciList::Private*);
    bool fibonacci_list_insert_rows(FibonacciList::Private*, int, int);
    bool fibonacci_list_remove_rows(FibonacciList::Private*, int, int);
    bool fibonacci_list_can_fetch_more(const FibonacciList::Private*);
    void fibonacci_list_fetch_more(FibonacciList::Private*);
}
int FibonacciList::columnCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : 2;
}

bool FibonacciList::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

int FibonacciList::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : fibonacci_list_row_count(m_d);
}

bool FibonacciList::insertRows(int row, int count, const QModelIndex &parent)
{
    return fibonacci_list_insert_rows(m_d, row, count);
}

bool FibonacciList::removeRows(int row, int count, const QModelIndex &parent)
{
    return fibonacci_list_remove_rows(m_d, row, count);
}

QModelIndex FibonacciList::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid() && row >= 0 && row < rowCount(parent) && column >= 0 && column < 2) {
        return createIndex(row, column, (quintptr)row);
    }
    return QModelIndex();
}

QModelIndex FibonacciList::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool FibonacciList::canFetchMore(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : fibonacci_list_can_fetch_more(m_d);
}

void FibonacciList::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid()) {
        fibonacci_list_fetch_more(m_d);
    }
}

void FibonacciList::sort(int column, Qt::SortOrder order)
{
    fibonacci_list_sort(m_d, column, order);
}
Qt::ItemFlags FibonacciList::flags(const QModelIndex &i) const
{
    auto flags = QAbstractItemModel::flags(i);
    return flags;
}

QVariant FibonacciList::fibonacciNumber(int row) const
{
    QVariant v;
    v = fibonacci_list_data_fibonacci_number(m_d, row);
    return v;
}

QVariant FibonacciList::row(int row) const
{
    QVariant v;
    v = fibonacci_list_data_row(m_d, row);
    return v;
}

QVariant FibonacciList::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(rowCount(index.parent()) > index.row());
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::UserRole + 0:
            return fibonacciNumber(index.row());
        case Qt::DisplayRole:
        case Qt::UserRole + 1:
            return row(index.row());
        }
    case 1:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 0:
            return fibonacciNumber(index.row());
        }
    }
    return QVariant();
}

QHash<int, QByteArray> FibonacciList::roleNames() const {
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(Qt::UserRole + 0, "fibonacciNumber");
    names.insert(Qt::UserRole + 1, "row");
    return names;
}
QVariant FibonacciList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    return m_headerData.value(qMakePair(section, (Qt::ItemDataRole)role), role == Qt::DisplayRole ?QString::number(section + 1) :QVariant());
}

bool FibonacciList::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation != Qt::Horizontal) {
        return false;
    }
    m_headerData.insert(qMakePair(section, (Qt::ItemDataRole)role), value);
    return true;
}

extern "C" {
    FibonacciList::Private* fibonacci_list_new(FibonacciList*,
        void (*)(const FibonacciList*),
        void (*)(FibonacciList*, quintptr, quintptr),
        void (*)(FibonacciList*),
        void (*)(FibonacciList*),
        void (*)(FibonacciList*, int, int),
        void (*)(FibonacciList*),
        void (*)(FibonacciList*, int, int),
        void (*)(FibonacciList*));
    void fibonacci_list_free(FibonacciList::Private*);
};

extern "C" {
    void file_system_tree_data_file_icon(const FileSystemTree::Private*, quintptr, QByteArray*, qbytearray_set);
    void file_system_tree_data_file_name(const FileSystemTree::Private*, quintptr, QString*, qstring_set);
    void file_system_tree_data_file_path(const FileSystemTree::Private*, quintptr, QString*, qstring_set);
    qint32 file_system_tree_data_file_permissions(const FileSystemTree::Private*, quintptr);
    option_quint64 file_system_tree_data_file_size(const FileSystemTree::Private*, quintptr);
    qint32 file_system_tree_data_file_type(const FileSystemTree::Private*, quintptr);
    void file_system_tree_sort(FileSystemTree::Private*, unsigned char column, Qt::SortOrder order = Qt::AscendingOrder);

    int file_system_tree_row_count(const FileSystemTree::Private*, quintptr, bool);
    bool file_system_tree_can_fetch_more(const FileSystemTree::Private*, quintptr, bool);
    void file_system_tree_fetch_more(FileSystemTree::Private*, quintptr, bool);
    quintptr file_system_tree_index(const FileSystemTree::Private*, quintptr, bool, int);
    qmodelindex_t file_system_tree_parent(const FileSystemTree::Private*, quintptr);
    int file_system_tree_row(const FileSystemTree::Private*, quintptr);
}
int FileSystemTree::columnCount(const QModelIndex &) const
{
    return 5;
}

bool FileSystemTree::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

int FileSystemTree::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return 0;
    }
    return file_system_tree_row_count(m_d, parent.internalId(), parent.isValid());
}

bool FileSystemTree::insertRows(int, int, const QModelIndex &)
{
    return false; // not supported yet
}

bool FileSystemTree::removeRows(int, int, const QModelIndex &)
{
    return false; // not supported yet
}

QModelIndex FileSystemTree::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= 5) {
        return QModelIndex();
    }
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }
    if (row >= rowCount(parent)) {
        return QModelIndex();
    }
    const quintptr id = file_system_tree_index(m_d, parent.internalId(), parent.isValid(), row);
    return createIndex(row, column, id);
}

QModelIndex FileSystemTree::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }
    const qmodelindex_t parent = file_system_tree_parent(m_d, index.internalId());
    return parent.row >= 0 ?createIndex(parent.row, 0, parent.id) :QModelIndex();
}

bool FileSystemTree::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return false;
    }
    return file_system_tree_can_fetch_more(m_d, parent.internalId(), parent.isValid());
}

void FileSystemTree::fetchMore(const QModelIndex &parent)
{
    file_system_tree_fetch_more(m_d, parent.internalId(), parent.isValid());
}

void FileSystemTree::sort(int column, Qt::SortOrder order)
{
    file_system_tree_sort(m_d, column, order);
}
Qt::ItemFlags FileSystemTree::flags(const QModelIndex &i) const
{
    auto flags = QAbstractItemModel::flags(i);
    return flags;
}

QVariant FileSystemTree::fileIcon(const QModelIndex& index) const
{
    QVariant v;
    QByteArray b;
    file_system_tree_data_file_icon(m_d, index.internalId(), &b, set_qbytearray);
    if (!b.isNull()) v.setValue<QByteArray>(b);
    return v;
}

QVariant FileSystemTree::fileName(const QModelIndex& index) const
{
    QVariant v;
    QString s;
    file_system_tree_data_file_name(m_d, index.internalId(), &s, set_qstring);
    if (!s.isNull()) v.setValue<QString>(s);
    return v;
}

QVariant FileSystemTree::filePath(const QModelIndex& index) const
{
    QVariant v;
    QString s;
    file_system_tree_data_file_path(m_d, index.internalId(), &s, set_qstring);
    if (!s.isNull()) v.setValue<QString>(s);
    return v;
}

QVariant FileSystemTree::filePermissions(const QModelIndex& index) const
{
    QVariant v;
    v = file_system_tree_data_file_permissions(m_d, index.internalId());
    return v;
}

QVariant FileSystemTree::fileSize(const QModelIndex& index) const
{
    QVariant v;
    v = file_system_tree_data_file_size(m_d, index.internalId());
    return v;
}

QVariant FileSystemTree::fileType(const QModelIndex& index) const
{
    QVariant v;
    v = file_system_tree_data_file_type(m_d, index.internalId());
    return v;
}

QVariant FileSystemTree::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(rowCount(index.parent()) > index.row());
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::DecorationRole:
        case Qt::UserRole + 0:
            return fileIcon(index);
        case Qt::DisplayRole:
        case Qt::UserRole + 1:
            return fileName(index);
        case Qt::UserRole + 2:
            return filePath(index);
        case Qt::UserRole + 3:
            return filePermissions(index);
        case Qt::UserRole + 4:
            return fileSize(index);
        case Qt::UserRole + 5:
            return fileType(index);
        }
    case 1:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 4:
            return fileSize(index);
        }
    case 2:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 2:
            return filePath(index);
        }
    case 3:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 3:
            return filePermissions(index);
        }
    case 4:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 5:
            return fileType(index);
        }
    }
    return QVariant();
}

QHash<int, QByteArray> FileSystemTree::roleNames() const {
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(Qt::UserRole + 0, "fileIcon");
    names.insert(Qt::UserRole + 1, "fileName");
    names.insert(Qt::UserRole + 2, "filePath");
    names.insert(Qt::UserRole + 3, "filePermissions");
    names.insert(Qt::UserRole + 4, "fileSize");
    names.insert(Qt::UserRole + 5, "fileType");
    return names;
}
QVariant FileSystemTree::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    return m_headerData.value(qMakePair(section, (Qt::ItemDataRole)role), role == Qt::DisplayRole ?QString::number(section + 1) :QVariant());
}

bool FileSystemTree::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation != Qt::Horizontal) {
        return false;
    }
    m_headerData.insert(qMakePair(section, (Qt::ItemDataRole)role), value);
    return true;
}

extern "C" {
    FileSystemTree::Private* file_system_tree_new(FileSystemTree*, void (*)(FileSystemTree*),
        void (*)(const FileSystemTree*, quintptr, bool),
        void (*)(FileSystemTree*, quintptr, quintptr),
        void (*)(FileSystemTree*),
        void (*)(FileSystemTree*),
        void (*)(FileSystemTree*, option_quintptr, int, int),
        void (*)(FileSystemTree*),
        void (*)(FileSystemTree*, option_quintptr, int, int),
        void (*)(FileSystemTree*));
    void file_system_tree_free(FileSystemTree::Private*);
    void file_system_tree_path_get(const FileSystemTree::Private*, QString*, qstring_set);
    void file_system_tree_path_set(FileSystemTree::Private*, qstring_t);
    void file_system_tree_path_set_none(FileSystemTree::Private*);
};

extern "C" {
    void processes_data_cmd(const Processes::Private*, quintptr, QString*, qstring_set);
    quint8 processes_data_cpu_percentage(const Processes::Private*, quintptr);
    float processes_data_cpu_usage(const Processes::Private*, quintptr);
    quint64 processes_data_memory(const Processes::Private*, quintptr);
    void processes_data_name(const Processes::Private*, quintptr, QString*, qstring_set);
    uint processes_data_pid(const Processes::Private*, quintptr);
    uint processes_data_uid(const Processes::Private*, quintptr);
    void processes_sort(Processes::Private*, unsigned char column, Qt::SortOrder order = Qt::AscendingOrder);

    int processes_row_count(const Processes::Private*, quintptr, bool);
    bool processes_can_fetch_more(const Processes::Private*, quintptr, bool);
    void processes_fetch_more(Processes::Private*, quintptr, bool);
    quintptr processes_index(const Processes::Private*, quintptr, bool, int);
    qmodelindex_t processes_parent(const Processes::Private*, quintptr);
    int processes_row(const Processes::Private*, quintptr);
}
int Processes::columnCount(const QModelIndex &) const
{
    return 3;
}

bool Processes::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

int Processes::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return 0;
    }
    return processes_row_count(m_d, parent.internalId(), parent.isValid());
}

bool Processes::insertRows(int, int, const QModelIndex &)
{
    return false; // not supported yet
}

bool Processes::removeRows(int, int, const QModelIndex &)
{
    return false; // not supported yet
}

QModelIndex Processes::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= 3) {
        return QModelIndex();
    }
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }
    if (row >= rowCount(parent)) {
        return QModelIndex();
    }
    const quintptr id = processes_index(m_d, parent.internalId(), parent.isValid(), row);
    return createIndex(row, column, id);
}

QModelIndex Processes::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }
    const qmodelindex_t parent = processes_parent(m_d, index.internalId());
    return parent.row >= 0 ?createIndex(parent.row, 0, parent.id) :QModelIndex();
}

bool Processes::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return false;
    }
    return processes_can_fetch_more(m_d, parent.internalId(), parent.isValid());
}

void Processes::fetchMore(const QModelIndex &parent)
{
    processes_fetch_more(m_d, parent.internalId(), parent.isValid());
}

void Processes::sort(int column, Qt::SortOrder order)
{
    processes_sort(m_d, column, order);
}
Qt::ItemFlags Processes::flags(const QModelIndex &i) const
{
    auto flags = QAbstractItemModel::flags(i);
    return flags;
}

QVariant Processes::cmd(const QModelIndex& index) const
{
    QVariant v;
    QString s;
    processes_data_cmd(m_d, index.internalId(), &s, set_qstring);
    if (!s.isNull()) v.setValue<QString>(s);
    return v;
}

QVariant Processes::cpuPercentage(const QModelIndex& index) const
{
    QVariant v;
    v = processes_data_cpu_percentage(m_d, index.internalId());
    return v;
}

QVariant Processes::cpuUsage(const QModelIndex& index) const
{
    QVariant v;
    v = processes_data_cpu_usage(m_d, index.internalId());
    return v;
}

QVariant Processes::memory(const QModelIndex& index) const
{
    QVariant v;
    v = processes_data_memory(m_d, index.internalId());
    return v;
}

QVariant Processes::name(const QModelIndex& index) const
{
    QVariant v;
    QString s;
    processes_data_name(m_d, index.internalId(), &s, set_qstring);
    if (!s.isNull()) v.setValue<QString>(s);
    return v;
}

QVariant Processes::pid(const QModelIndex& index) const
{
    QVariant v;
    v = processes_data_pid(m_d, index.internalId());
    return v;
}

QVariant Processes::uid(const QModelIndex& index) const
{
    QVariant v;
    v = processes_data_uid(m_d, index.internalId());
    return v;
}

QVariant Processes::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(rowCount(index.parent()) > index.row());
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::UserRole + 0:
            return cmd(index);
        case Qt::UserRole + 1:
            return cpuPercentage(index);
        case Qt::UserRole + 2:
            return cpuUsage(index);
        case Qt::UserRole + 3:
            return memory(index);
        case Qt::DisplayRole:
        case Qt::UserRole + 4:
            return name(index);
        case Qt::ToolTipRole:
        case Qt::UserRole + 5:
            return pid(index);
        case Qt::UserRole + 6:
            return uid(index);
        }
    case 1:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 2:
            return cpuUsage(index);
        }
    case 2:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::UserRole + 3:
            return memory(index);
        }
    }
    return QVariant();
}

QHash<int, QByteArray> Processes::roleNames() const {
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(Qt::UserRole + 0, "cmd");
    names.insert(Qt::UserRole + 1, "cpuPercentage");
    names.insert(Qt::UserRole + 2, "cpuUsage");
    names.insert(Qt::UserRole + 3, "memory");
    names.insert(Qt::UserRole + 4, "name");
    names.insert(Qt::UserRole + 5, "pid");
    names.insert(Qt::UserRole + 6, "uid");
    return names;
}
QVariant Processes::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    return m_headerData.value(qMakePair(section, (Qt::ItemDataRole)role), role == Qt::DisplayRole ?QString::number(section + 1) :QVariant());
}

bool Processes::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation != Qt::Horizontal) {
        return false;
    }
    m_headerData.insert(qMakePair(section, (Qt::ItemDataRole)role), value);
    return true;
}

extern "C" {
    Processes::Private* processes_new(Processes*, void (*)(Processes*),
        void (*)(const Processes*, quintptr, bool),
        void (*)(Processes*, quintptr, quintptr),
        void (*)(Processes*),
        void (*)(Processes*),
        void (*)(Processes*, option_quintptr, int, int),
        void (*)(Processes*),
        void (*)(Processes*, option_quintptr, int, int),
        void (*)(Processes*));
    void processes_free(Processes::Private*);
    bool processes_active_get(const Processes::Private*);
    void processes_active_set(Processes::Private*, bool);
};

extern "C" {
    float time_series_data_cos(const TimeSeries::Private*, int);
    bool time_series_set_data_cos(TimeSeries::Private*, int, float);
    float time_series_data_sin(const TimeSeries::Private*, int);
    bool time_series_set_data_sin(TimeSeries::Private*, int, float);
    float time_series_data_time(const TimeSeries::Private*, int);
    bool time_series_set_data_time(TimeSeries::Private*, int, float);
    void time_series_sort(TimeSeries::Private*, unsigned char column, Qt::SortOrder order = Qt::AscendingOrder);

    int time_series_row_count(const TimeSeries::Private*);
    bool time_series_insert_rows(TimeSeries::Private*, int, int);
    bool time_series_remove_rows(TimeSeries::Private*, int, int);
    bool time_series_can_fetch_more(const TimeSeries::Private*);
    void time_series_fetch_more(TimeSeries::Private*);
}
int TimeSeries::columnCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : 3;
}

bool TimeSeries::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

int TimeSeries::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : time_series_row_count(m_d);
}

bool TimeSeries::insertRows(int row, int count, const QModelIndex &parent)
{
    return time_series_insert_rows(m_d, row, count);
}

bool TimeSeries::removeRows(int row, int count, const QModelIndex &parent)
{
    return time_series_remove_rows(m_d, row, count);
}

QModelIndex TimeSeries::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid() && row >= 0 && row < rowCount(parent) && column >= 0 && column < 3) {
        return createIndex(row, column, (quintptr)row);
    }
    return QModelIndex();
}

QModelIndex TimeSeries::parent(const QModelIndex &) const
{
    return QModelIndex();
}

bool TimeSeries::canFetchMore(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : time_series_can_fetch_more(m_d);
}

void TimeSeries::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid()) {
        time_series_fetch_more(m_d);
    }
}

void TimeSeries::sort(int column, Qt::SortOrder order)
{
    time_series_sort(m_d, column, order);
}
Qt::ItemFlags TimeSeries::flags(const QModelIndex &i) const
{
    auto flags = QAbstractItemModel::flags(i);
    if (i.column() == 0) {
        flags |= Qt::ItemIsEditable;
    }
    if (i.column() == 1) {
        flags |= Qt::ItemIsEditable;
    }
    if (i.column() == 2) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

QVariant TimeSeries::cos(int row) const
{
    QVariant v;
    v = time_series_data_cos(m_d, row);
    return v;
}

bool TimeSeries::setCos(int row, const QVariant& value)
{
    bool set = false;
    set = time_series_set_data_cos(m_d, row, value.value<float>());
    if (set) {
        QModelIndex index = createIndex(row, 0, row);
        emit dataChanged(index, index);
    }
    return set;
}

QVariant TimeSeries::sin(int row) const
{
    QVariant v;
    v = time_series_data_sin(m_d, row);
    return v;
}

bool TimeSeries::setSin(int row, const QVariant& value)
{
    bool set = false;
    set = time_series_set_data_sin(m_d, row, value.value<float>());
    if (set) {
        QModelIndex index = createIndex(row, 0, row);
        emit dataChanged(index, index);
    }
    return set;
}

QVariant TimeSeries::time(int row) const
{
    QVariant v;
    v = time_series_data_time(m_d, row);
    return v;
}

bool TimeSeries::setTime(int row, const QVariant& value)
{
    bool set = false;
    set = time_series_set_data_time(m_d, row, value.value<float>());
    if (set) {
        QModelIndex index = createIndex(row, 0, row);
        emit dataChanged(index, index);
    }
    return set;
}

QVariant TimeSeries::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(rowCount(index.parent()) > index.row());
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::UserRole + 0:
            return cos(index.row());
        case Qt::UserRole + 1:
            return sin(index.row());
        case Qt::DisplayRole:
        case Qt::EditRole:
        case Qt::UserRole + 2:
            return time(index.row());
        }
    case 1:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case Qt::UserRole + 1:
            return sin(index.row());
        }
    case 2:
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case Qt::UserRole + 0:
            return cos(index.row());
        }
    }
    return QVariant();
}

QHash<int, QByteArray> TimeSeries::roleNames() const {
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(Qt::UserRole + 0, "cos");
    names.insert(Qt::UserRole + 1, "sin");
    names.insert(Qt::UserRole + 2, "time");
    return names;
}
QVariant TimeSeries::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    return m_headerData.value(qMakePair(section, (Qt::ItemDataRole)role), role == Qt::DisplayRole ?QString::number(section + 1) :QVariant());
}

bool TimeSeries::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation != Qt::Horizontal) {
        return false;
    }
    m_headerData.insert(qMakePair(section, (Qt::ItemDataRole)role), value);
    return true;
}

bool TimeSeries::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 0) {
        if (role == Qt::UserRole + 0) {
            return setCos(index.row(), value);
        }
        if (role == Qt::UserRole + 1) {
            return setSin(index.row(), value);
        }
        if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::UserRole + 2) {
            return setTime(index.row(), value);
        }
    }
    if (index.column() == 1) {
        if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::UserRole + 1) {
            return setSin(index.row(), value);
        }
    }
    if (index.column() == 2) {
        if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::UserRole + 0) {
            return setCos(index.row(), value);
        }
    }
    return false;
}

extern "C" {
    TimeSeries::Private* time_series_new(TimeSeries*,
        void (*)(const TimeSeries*),
        void (*)(TimeSeries*, quintptr, quintptr),
        void (*)(TimeSeries*),
        void (*)(TimeSeries*),
        void (*)(TimeSeries*, int, int),
        void (*)(TimeSeries*),
        void (*)(TimeSeries*, int, int),
        void (*)(TimeSeries*));
    void time_series_free(TimeSeries::Private*);
};

Demo::Demo(bool /*owned*/, QObject *parent):
    QObject(parent),
    m_fibonacci(new Fibonacci(false, this)),
    m_fibonacciList(new FibonacciList(false, this)),
    m_fileSystemTree(new FileSystemTree(false, this)),
    m_processes(new Processes(false, this)),
    m_timeSeries(new TimeSeries(false, this)),
    m_d(0),
    m_ownsPrivate(false)
{
}

Demo::Demo(QObject *parent):
    QObject(parent),
    m_fibonacci(new Fibonacci(false, this)),
    m_fibonacciList(new FibonacciList(false, this)),
    m_fileSystemTree(new FileSystemTree(false, this)),
    m_processes(new Processes(false, this)),
    m_timeSeries(new TimeSeries(false, this)),
    m_d(demo_new(this, m_fibonacci,
        fibonacciInputChanged,
        fibonacciResultChanged, m_fibonacciList,
        [](const FibonacciList* o) {
            emit o->newDataReady(QModelIndex());
        },
        [](FibonacciList* o, quintptr first, quintptr last) {
            o->dataChanged(o->createIndex(first, 0, first),
                       o->createIndex(last, 1, last));
        },
        [](FibonacciList* o) {
            o->beginResetModel();
        },
        [](FibonacciList* o) {
            o->endResetModel();
        },
        [](FibonacciList* o, int first, int last) {
            o->beginInsertRows(QModelIndex(), first, last);
        },
        [](FibonacciList* o) {
            o->endInsertRows();
        },
        [](FibonacciList* o, int first, int last) {
            o->beginRemoveRows(QModelIndex(), first, last);
        },
        [](FibonacciList* o) {
            o->endRemoveRows();
        }
, m_fileSystemTree,
        fileSystemTreePathChanged,
        [](const FileSystemTree* o, quintptr id, bool valid) {
            if (valid) {
                int row = file_system_tree_row(o->m_d, id);
                emit o->newDataReady(o->createIndex(row, 0, id));
            } else {
                emit o->newDataReady(QModelIndex());
            }
        },
        [](FileSystemTree* o, quintptr first, quintptr last) {
            quintptr frow = file_system_tree_row(o->m_d, first);
            quintptr lrow = file_system_tree_row(o->m_d, first);
            o->dataChanged(o->createIndex(frow, 0, first),
                       o->createIndex(lrow, 4, last));
        },
        [](FileSystemTree* o) {
            o->beginResetModel();
        },
        [](FileSystemTree* o) {
            o->endResetModel();
        },
        [](FileSystemTree* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = file_system_tree_row(o->m_d, id.value);
                o->beginInsertRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginInsertRows(QModelIndex(), first, last);
            }
        },
        [](FileSystemTree* o) {
            o->endInsertRows();
        },
        [](FileSystemTree* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = file_system_tree_row(o->m_d, id.value);
                o->beginRemoveRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginRemoveRows(QModelIndex(), first, last);
            }
        },
        [](FileSystemTree* o) {
            o->endRemoveRows();
        }
, m_processes,
        processesActiveChanged,
        [](const Processes* o, quintptr id, bool valid) {
            if (valid) {
                int row = processes_row(o->m_d, id);
                emit o->newDataReady(o->createIndex(row, 0, id));
            } else {
                emit o->newDataReady(QModelIndex());
            }
        },
        [](Processes* o, quintptr first, quintptr last) {
            quintptr frow = processes_row(o->m_d, first);
            quintptr lrow = processes_row(o->m_d, first);
            o->dataChanged(o->createIndex(frow, 0, first),
                       o->createIndex(lrow, 2, last));
        },
        [](Processes* o) {
            o->beginResetModel();
        },
        [](Processes* o) {
            o->endResetModel();
        },
        [](Processes* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = processes_row(o->m_d, id.value);
                o->beginInsertRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginInsertRows(QModelIndex(), first, last);
            }
        },
        [](Processes* o) {
            o->endInsertRows();
        },
        [](Processes* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = processes_row(o->m_d, id.value);
                o->beginRemoveRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginRemoveRows(QModelIndex(), first, last);
            }
        },
        [](Processes* o) {
            o->endRemoveRows();
        }
, m_timeSeries,
        [](const TimeSeries* o) {
            emit o->newDataReady(QModelIndex());
        },
        [](TimeSeries* o, quintptr first, quintptr last) {
            o->dataChanged(o->createIndex(first, 0, first),
                       o->createIndex(last, 2, last));
        },
        [](TimeSeries* o) {
            o->beginResetModel();
        },
        [](TimeSeries* o) {
            o->endResetModel();
        },
        [](TimeSeries* o, int first, int last) {
            o->beginInsertRows(QModelIndex(), first, last);
        },
        [](TimeSeries* o) {
            o->endInsertRows();
        },
        [](TimeSeries* o, int first, int last) {
            o->beginRemoveRows(QModelIndex(), first, last);
        },
        [](TimeSeries* o) {
            o->endRemoveRows();
        }
)),
    m_ownsPrivate(true)
{
    m_fibonacci->m_d = demo_fibonacci_get(m_d);
    m_fibonacciList->m_d = demo_fibonacci_list_get(m_d);
    m_fileSystemTree->m_d = demo_file_system_tree_get(m_d);
    m_processes->m_d = demo_processes_get(m_d);
    m_timeSeries->m_d = demo_time_series_get(m_d);
    connect(this->m_fibonacciList, &FibonacciList::newDataReady, this->m_fibonacciList, [this](const QModelIndex& i) {
        this->m_fibonacciList->fetchMore(i);
    }, Qt::QueuedConnection);
    connect(this->m_fileSystemTree, &FileSystemTree::newDataReady, this->m_fileSystemTree, [this](const QModelIndex& i) {
        this->m_fileSystemTree->fetchMore(i);
    }, Qt::QueuedConnection);
    connect(this->m_processes, &Processes::newDataReady, this->m_processes, [this](const QModelIndex& i) {
        this->m_processes->fetchMore(i);
    }, Qt::QueuedConnection);
    connect(this->m_timeSeries, &TimeSeries::newDataReady, this->m_timeSeries, [this](const QModelIndex& i) {
        this->m_timeSeries->fetchMore(i);
    }, Qt::QueuedConnection);
}

Demo::~Demo() {
    if (m_ownsPrivate) {
        demo_free(m_d);
    }
}
const Fibonacci* Demo::fibonacci() const
{
    return m_fibonacci;
}
Fibonacci* Demo::fibonacci()
{
    return m_fibonacci;
}
const FibonacciList* Demo::fibonacciList() const
{
    return m_fibonacciList;
}
FibonacciList* Demo::fibonacciList()
{
    return m_fibonacciList;
}
const FileSystemTree* Demo::fileSystemTree() const
{
    return m_fileSystemTree;
}
FileSystemTree* Demo::fileSystemTree()
{
    return m_fileSystemTree;
}
const Processes* Demo::processes() const
{
    return m_processes;
}
Processes* Demo::processes()
{
    return m_processes;
}
const TimeSeries* Demo::timeSeries() const
{
    return m_timeSeries;
}
TimeSeries* Demo::timeSeries()
{
    return m_timeSeries;
}
Fibonacci::Fibonacci(bool /*owned*/, QObject *parent):
    QObject(parent),
    m_d(0),
    m_ownsPrivate(false)
{
}

Fibonacci::Fibonacci(QObject *parent):
    QObject(parent),
    m_d(fibonacci_new(this,
        fibonacciInputChanged,
        fibonacciResultChanged)),
    m_ownsPrivate(true)
{
}

Fibonacci::~Fibonacci() {
    if (m_ownsPrivate) {
        fibonacci_free(m_d);
    }
}
quint32 Fibonacci::input() const
{
    return fibonacci_input_get(m_d);
}
void Fibonacci::setInput(uint v) {
    fibonacci_input_set(m_d, v);
}
quint64 Fibonacci::result() const
{
    return fibonacci_result_get(m_d);
}
FibonacciList::FibonacciList(bool /*owned*/, QObject *parent):
    QAbstractItemModel(parent),
    m_d(0),
    m_ownsPrivate(false)
{
    initHeaderData();
}

FibonacciList::FibonacciList(QObject *parent):
    QAbstractItemModel(parent),
    m_d(fibonacci_list_new(this,
        [](const FibonacciList* o) {
            emit o->newDataReady(QModelIndex());
        },
        [](FibonacciList* o, quintptr first, quintptr last) {
            o->dataChanged(o->createIndex(first, 0, first),
                       o->createIndex(last, 1, last));
        },
        [](FibonacciList* o) {
            o->beginResetModel();
        },
        [](FibonacciList* o) {
            o->endResetModel();
        },
        [](FibonacciList* o, int first, int last) {
            o->beginInsertRows(QModelIndex(), first, last);
        },
        [](FibonacciList* o) {
            o->endInsertRows();
        },
        [](FibonacciList* o, int first, int last) {
            o->beginRemoveRows(QModelIndex(), first, last);
        },
        [](FibonacciList* o) {
            o->endRemoveRows();
        }
)),
    m_ownsPrivate(true)
{
    connect(this, &FibonacciList::newDataReady, this, [this](const QModelIndex& i) {
        this->fetchMore(i);
    }, Qt::QueuedConnection);
    initHeaderData();
}

FibonacciList::~FibonacciList() {
    if (m_ownsPrivate) {
        fibonacci_list_free(m_d);
    }
}
void FibonacciList::initHeaderData() {
    m_headerData.insert(qMakePair(0, Qt::DisplayRole), QVariant("row"));
    m_headerData.insert(qMakePair(1, Qt::DisplayRole), QVariant("fibonacciNumber"));
}
FileSystemTree::FileSystemTree(bool /*owned*/, QObject *parent):
    QAbstractItemModel(parent),
    m_d(0),
    m_ownsPrivate(false)
{
    initHeaderData();
}

FileSystemTree::FileSystemTree(QObject *parent):
    QAbstractItemModel(parent),
    m_d(file_system_tree_new(this,
        fileSystemTreePathChanged,
        [](const FileSystemTree* o, quintptr id, bool valid) {
            if (valid) {
                int row = file_system_tree_row(o->m_d, id);
                emit o->newDataReady(o->createIndex(row, 0, id));
            } else {
                emit o->newDataReady(QModelIndex());
            }
        },
        [](FileSystemTree* o, quintptr first, quintptr last) {
            quintptr frow = file_system_tree_row(o->m_d, first);
            quintptr lrow = file_system_tree_row(o->m_d, first);
            o->dataChanged(o->createIndex(frow, 0, first),
                       o->createIndex(lrow, 4, last));
        },
        [](FileSystemTree* o) {
            o->beginResetModel();
        },
        [](FileSystemTree* o) {
            o->endResetModel();
        },
        [](FileSystemTree* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = file_system_tree_row(o->m_d, id.value);
                o->beginInsertRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginInsertRows(QModelIndex(), first, last);
            }
        },
        [](FileSystemTree* o) {
            o->endInsertRows();
        },
        [](FileSystemTree* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = file_system_tree_row(o->m_d, id.value);
                o->beginRemoveRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginRemoveRows(QModelIndex(), first, last);
            }
        },
        [](FileSystemTree* o) {
            o->endRemoveRows();
        }
)),
    m_ownsPrivate(true)
{
    connect(this, &FileSystemTree::newDataReady, this, [this](const QModelIndex& i) {
        this->fetchMore(i);
    }, Qt::QueuedConnection);
    initHeaderData();
}

FileSystemTree::~FileSystemTree() {
    if (m_ownsPrivate) {
        file_system_tree_free(m_d);
    }
}
void FileSystemTree::initHeaderData() {
    m_headerData.insert(qMakePair(0, Qt::DisplayRole), QVariant("fileName"));
    m_headerData.insert(qMakePair(1, Qt::DisplayRole), QVariant("fileSize"));
    m_headerData.insert(qMakePair(2, Qt::DisplayRole), QVariant("filePath"));
    m_headerData.insert(qMakePair(3, Qt::DisplayRole), QVariant("filePermissions"));
    m_headerData.insert(qMakePair(4, Qt::DisplayRole), QVariant("fileType"));
}
QString FileSystemTree::path() const
{
    QString v;
    file_system_tree_path_get(m_d, &v, set_qstring);
    return v;
}
void FileSystemTree::setPath(const QString& v) {
    if (v.isNull()) {
        file_system_tree_path_set_none(m_d);
    } else {
        file_system_tree_path_set(m_d, v);
    }
}
Processes::Processes(bool /*owned*/, QObject *parent):
    QAbstractItemModel(parent),
    m_d(0),
    m_ownsPrivate(false)
{
    initHeaderData();
}

Processes::Processes(QObject *parent):
    QAbstractItemModel(parent),
    m_d(processes_new(this,
        processesActiveChanged,
        [](const Processes* o, quintptr id, bool valid) {
            if (valid) {
                int row = processes_row(o->m_d, id);
                emit o->newDataReady(o->createIndex(row, 0, id));
            } else {
                emit o->newDataReady(QModelIndex());
            }
        },
        [](Processes* o, quintptr first, quintptr last) {
            quintptr frow = processes_row(o->m_d, first);
            quintptr lrow = processes_row(o->m_d, first);
            o->dataChanged(o->createIndex(frow, 0, first),
                       o->createIndex(lrow, 2, last));
        },
        [](Processes* o) {
            o->beginResetModel();
        },
        [](Processes* o) {
            o->endResetModel();
        },
        [](Processes* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = processes_row(o->m_d, id.value);
                o->beginInsertRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginInsertRows(QModelIndex(), first, last);
            }
        },
        [](Processes* o) {
            o->endInsertRows();
        },
        [](Processes* o, option_quintptr id, int first, int last) {
            if (id.some) {
                int row = processes_row(o->m_d, id.value);
                o->beginRemoveRows(o->createIndex(row, 0, id.value), first, last);
            } else {
                o->beginRemoveRows(QModelIndex(), first, last);
            }
        },
        [](Processes* o) {
            o->endRemoveRows();
        }
)),
    m_ownsPrivate(true)
{
    connect(this, &Processes::newDataReady, this, [this](const QModelIndex& i) {
        this->fetchMore(i);
    }, Qt::QueuedConnection);
    initHeaderData();
}

Processes::~Processes() {
    if (m_ownsPrivate) {
        processes_free(m_d);
    }
}
void Processes::initHeaderData() {
    m_headerData.insert(qMakePair(0, Qt::DisplayRole), QVariant("name"));
    m_headerData.insert(qMakePair(1, Qt::DisplayRole), QVariant("cpuUsage"));
    m_headerData.insert(qMakePair(2, Qt::DisplayRole), QVariant("memory"));
}
bool Processes::active() const
{
    return processes_active_get(m_d);
}
void Processes::setActive(bool v) {
    processes_active_set(m_d, v);
}
TimeSeries::TimeSeries(bool /*owned*/, QObject *parent):
    QAbstractItemModel(parent),
    m_d(0),
    m_ownsPrivate(false)
{
    initHeaderData();
}

TimeSeries::TimeSeries(QObject *parent):
    QAbstractItemModel(parent),
    m_d(time_series_new(this,
        [](const TimeSeries* o) {
            emit o->newDataReady(QModelIndex());
        },
        [](TimeSeries* o, quintptr first, quintptr last) {
            o->dataChanged(o->createIndex(first, 0, first),
                       o->createIndex(last, 2, last));
        },
        [](TimeSeries* o) {
            o->beginResetModel();
        },
        [](TimeSeries* o) {
            o->endResetModel();
        },
        [](TimeSeries* o, int first, int last) {
            o->beginInsertRows(QModelIndex(), first, last);
        },
        [](TimeSeries* o) {
            o->endInsertRows();
        },
        [](TimeSeries* o, int first, int last) {
            o->beginRemoveRows(QModelIndex(), first, last);
        },
        [](TimeSeries* o) {
            o->endRemoveRows();
        }
)),
    m_ownsPrivate(true)
{
    connect(this, &TimeSeries::newDataReady, this, [this](const QModelIndex& i) {
        this->fetchMore(i);
    }, Qt::QueuedConnection);
    initHeaderData();
}

TimeSeries::~TimeSeries() {
    if (m_ownsPrivate) {
        time_series_free(m_d);
    }
}
void TimeSeries::initHeaderData() {
    m_headerData.insert(qMakePair(0, Qt::DisplayRole), QVariant("time"));
    m_headerData.insert(qMakePair(1, Qt::DisplayRole), QVariant("sin"));
    m_headerData.insert(qMakePair(2, Qt::DisplayRole), QVariant("cos"));
}
