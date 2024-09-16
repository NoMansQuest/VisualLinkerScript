#ifndef VIOLATIONS_MODEL_H_
#define VIOLATIONS_MODEL_H_

#include <QAbstractItemModel>
#include "CViolationEntryBase.h"

/// @brief Violations model is the main model fed to the QAbstractTreeView.
class QViolationsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /// @brief Default constructor
    explicit QViolationsModel(const QString& data, QObject* parent = nullptr);

    /// @brief Default destructor.
    ~QViolationsModel() = default;

    /// @brief Returns variant data.
    QVariant data(const QModelIndex& index, int role) const override;

    /// @brief Returns flags associated with a given row;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    /// @brief Returns header data associated with a given section.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /// @brief Returns index of a given row and column
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /// @brief Returns the parent of a given row;
    QModelIndex parent(const QModelIndex& index) const override;

    /// @breif Reports back the row count.
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /// @brief Reports back the column count.
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

private:    
    CViolationEntryBase* rootItem;
};

#endif // VIOLATIONS_MODEL_H_