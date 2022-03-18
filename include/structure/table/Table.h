#pragma once

// Represents a 1d, 2d, 3d table layout or a list of the previous.


#include <array>
#include <vector>
#include <stdexcept>
#include <memory>
#include <functional>
#include <string>
#include <enum.h>


namespace ieml::structure {

BETTER_ENUM(TableType, char, CELL, TABLEND, TABLESET)

template<class V>
class TableNd;

template<class V>
class Table {
public:
    virtual ~Table() = default;
    
    std::string uid() const {return "table_" + std::to_string(std::hash<std::string>{}(table_name()->uid()));};

    typedef std::tuple<size_t, std::vector<std::array<size_t, 3>>> Shape;

    virtual TableType get_type() const = 0;

    virtual std::vector<const TableNd<V>*> get_sub_tables() const = 0;

    virtual Shape getShape() const = 0;

    virtual const TableNd<V>* at_table_set_dim(size_t table_set_dim) const = 0;

    virtual size_t getCellsNumber() const = 0;

    virtual std::vector<V> getCells() const = 0;

    virtual const V& table_name() const = 0;

    virtual const V& table_dim_name(size_t table_set_dim) const = 0;

    virtual const V& header_at(size_t table_set_dim, size_t dim, size_t index) const = 0;

    virtual const std::vector<V>& getHeaders(size_t table_set_dim, size_t dim) const = 0;
    
    virtual const V& at(size_t table_set_dim, const std::array<size_t, 3>& coord) const = 0;

    virtual unsigned char ndim(size_t) const = 0;
};

template<class V, unsigned char N>
class TableNd_ ;

template<class V>
class TableNd : public Table<V> {
public:

    static const TableNd* create(
            const std::vector<V>& values, 
            const std::vector<size_t>& shape,
            const std::vector<std::vector<V>>& headers,
            const V& table_name) {
        unsigned char ndim = shape.size();
        if (ndim == 1) {
            std::array<size_t, 1> shape_arr{shape[0]};
            std::array<std::vector<V>, 1> headers_arr{headers[0]};
            return new TableNd_<V, 1>(values, shape_arr, headers_arr, table_name);
        } else if (ndim == 2) {
            std::array<size_t, 2> shape_arr{shape[0], shape[1]};
            std::array<std::vector<V>, 2> headers_arr{headers[0], headers[1]};
            return new TableNd_<V, 2>(values, shape_arr, headers_arr, table_name);
        } else {
            std::array<size_t, 3> shape_arr{shape[0], shape[1], shape[2]};
            std::array<std::vector<V>, 3> headers_arr{headers[0], headers[1], headers[2]};
            return new TableNd_<V, 3>(values, shape_arr, headers_arr, table_name);
        }
    }

    virtual TableType get_type() const override {return TableType::TABLEND;}
    virtual std::array<size_t, 3> getShapeTableNd() const = 0;

    virtual typename Table<V>::Shape getShape() const override {
        return {1, std::vector<std::array<size_t, 3>>{getShapeTableNd()}};
    }

    virtual const V& table_name() const override = 0;

    virtual const V& table_dim_name(size_t) const override {
        return table_name();
    }

    virtual std::vector<const TableNd<V>*> get_sub_tables() const override {
        return {this};
    };

    virtual const TableNd<V>* at_table_set_dim(size_t) const override {
        return this;
    }

};

template<class V, unsigned char N>
class TableNd_ : public TableNd<V> {
public:
    TableNd_(const std::vector<V>& values, 
             const std::array<size_t, N>& shape,
             const std::array<std::vector<V>, N>& headers,
             const V& table_name) : 
        values_(values), 
        shape_(shape),
        headers_(headers),
        table_name_(table_name) {
            static_assert(N <= 3);

            if(getCellsNumber() != values_.size())
                throw std::invalid_argument("Table dimension and values does not match.");

            for (size_t i = 0; i < N; i++) {
                if (headers_[i].size() != shape_[i])
                    throw std::invalid_argument("Table dimension and headers number does not match.");

            }
    }
    virtual unsigned char ndim(size_t) const override {return N;};

    virtual std::array<size_t, 3> getShapeTableNd() const override {
        std::array<size_t, 3> res = {1, 1, 1};
        for (size_t i = 0; i < N; i++)
            res[i] = shape_[i];
        return res;
    }

    virtual size_t getCellsNumber() const override {
        size_t res = 1;
        for (size_t i = 0; i < N; i++)
            res *= shape_[i];
        return res;
    };

    virtual std::vector<V> getCells() const override {
        return values_;
    };

    virtual const V& header_at(size_t, size_t dim, size_t index) const override {
        return headers_[dim][index];
    }
    virtual const std::vector<V>& getHeaders(size_t, size_t dim) const override {
        return headers_[dim];
    };

    virtual const V& at(size_t, const std::array<size_t, 3>& coord) const override {
        size_t loc = 0;
        size_t inc = 1;
        for (size_t i = 0; i < N; i++) {
            loc += inc * coord[i];
            inc *= shape_[i];
        }
        return values_[loc];
    }

    virtual const V& table_name() const override {
        return table_name_;
    }
private:
    const std::vector<V> values_;
    const std::array<size_t, N> shape_;
    const std::array<std::vector<V>, N> headers_;
    const V table_name_;
};


template<class V>
class Cell: public TableNd_<V, 1> {
public:
    Cell(const V& value) : 
        TableNd_<V, 1>(
            std::vector<V>{value},
            std::array<size_t, 1>{1},
            std::array<std::vector<V>, 1>{std::vector<V>{value}},
            value
        ) {}
    
    virtual TableType get_type() const override {return TableType::CELL;}
};

template<class V>
class TableSet: public Table<V> {
public:
    typedef std::vector<const TableNd<V>*> Children;

    TableSet(const Children& children,
             const V& table_name) : 
        children_(children),
        table_name_(table_name) {}

    virtual TableType get_type() const override {return TableType::TABLESET;}

    virtual std::vector<const TableNd<V>*> get_sub_tables() const override {
        return children_;
    };

    virtual typename Table<V>::Shape getShape() const override {
        std::vector<std::array<size_t, 3>> res;
        for (const auto& child: children_) {
            res.push_back(child->getShapeTableNd());
        }
        return {children_.size(), res};
    };

    virtual size_t getCellsNumber() const override {
        size_t res = 0;
        for (const auto& child: children_) {
            res += child->getCellsNumber();
        }
        return res;
    };
    virtual std::vector<V> getCells() const override {
        std::vector<V> res;
        res.reserve(getCellsNumber());
        for (const auto& child: children_) {
            const auto child_cells = child->getCells();
            res.insert(res.end(), child_cells.begin(), child_cells.end());
        }
        return res;
    };

    virtual unsigned char ndim(size_t table_set_dim) const override {
        return children_[table_set_dim]->ndim(0);
    };

    virtual const V& header_at(size_t table_set_dim, size_t dim, size_t index) const override {
        return children_[table_set_dim]->header_at(0, dim, index);
    };
    virtual const std::vector<V>& getHeaders(size_t table_set_dim, size_t dim) const override {
        return children_[table_set_dim]->getHeaders(0, dim);
    };


    virtual const V& at(size_t table_set_dim, const std::array<size_t, 3>& coord) const override {
        return children_[table_set_dim]->at(0, coord);
    };

    virtual const V& table_name() const override {
        return table_name_;
    }

    virtual const V& table_dim_name(size_t table_set_dim) const override {
        return children_[table_set_dim]->table_name();
    }

    virtual const TableNd<V>* at_table_set_dim(size_t table_set_dim) const override {
        return children_[table_set_dim];
    }

private:
    const Children children_;

    const V table_name_;
};


}