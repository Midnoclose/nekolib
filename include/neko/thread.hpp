
/*
 * Nekolib: An extention to the standard libraries for ease of use
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <thread>

namespace neko {

// A javascript promise
template <typename TReturnArg>
class Promise {
    class InternalPromise {
        enum class State { kRunning, kCompleted, kThrew };
        State state = State::kRunning;

       public:
        InternalPromise(std::function<void(std::function<void(TReturnArg)>,
                                           std::function<void(std::exception)>)>
                            async_run) {
            std::async([&]() {
                try {
                    async_run(
                        [&](TReturnArg ret) {
                            this->return_arg = ret;
                            this->state = State::kCompleted;
                            this->TryDelete();
                        },
                        [&](std::exception err) {
                            this->exception = err;
                            this->state = State::kThrew;
                            this->TryDelete();
                        });
                } catch (std::exception err) {
                    this->exception = err;
                    this->state = State::kThrew;
                    this->TryDelete();
                } catch (...) {
                    this->exception = std::runtime_error("Unknown error threw");
                    this->state = State::kThrew;
                    this->TryDelete();
                }
            });
        }
        void Then(std::function<void(TReturnArg)> _then) {
            this->then = _then;
            this->TryFinish();
        }
        void Catch(std::function<void(std::exception)> _then) {
            this->catch_err = _then;
            this->TryFinish();
        }
        void Unlink() {
            this->linked = false;
            this->TryDelete();
        }
        // Internal
       private:
        void TryFinish() {
            if (this->finished) return;
            this->finished = true;
            if (this->status == State::kCompleted && this->then != nullptr) {
                return this->then(return_arg);
            } else if (this->status == State::kThrew &&
                       this->catch_err != nullptr)
                return this->catch_err(exception);
            this->finished = false;
        }
        void TryDelete() {
            this->TryFinish();
            if (this->linked) return;
            if (!this->finished) {
                if (this->status == State::kThrew)
                    throw std::runtime_error(
                        std::string("Uncaught promise exception: ") +
                        this->exception.what());
            } else
                delete this;
        }

       private:
        bool linked = true;
        // Ret
        TReturnArg return_arg;
        std::exception exception;
        // Funcs
        std::function<void(TReturnArg)> then;
        std::function<void(std::exception)> catch_err;
        bool finished;
    };
    InternalPromise* internal;

   public:
    Promise(std::function<void(std::function<void(TReturnArg)>,
                               std::function<void(std::exception)>)>
                async_run) {
        this->internal = new InternalPromise(async_run);
    }
    ~Promise() { this->internal->Unlink(); }

   public:
    Promise& Then(std::function<void(TReturnArg)> then) {
        this->internal->Then(then);
        return *this;
    }
    Promise& Catch(std::function<void(std::exception)> catch_err) {
        this->internal->Catch(catch_err);
        return *this;
    }
};

namespace thread {
using namespace std::chrono_literals;

class Loop {
   public:
    using MainFunc = std::function<void()>;
    using ErrFunc = std::function<void(std::exception)>;
    Loop() { this->Clear(); }
    ~Loop() { this->Clear(); }
    void Set(MainFunc f, int d = 0) {
        this->main_func = f;
        this->do_continue = true;
        std::thread wait([this]() {
            std::lock_guard<std::mutex> guard(this->lock);
            this->running = true;
            while (this->do_continue) {
                std::this_thread::sleep_for(this->wait_time);
                try {
                    this->main_func();
                } catch (std::exception e) {
                    this->err_func(e);
                }
            }
            this->running = false;
        });
        wait.detach();
    }
    void SetErr(ErrFunc f) { this->err_func = f; }
    void Clear() {
        this->main_func = []() {};
        this->err_func = [](auto) {};
        this->do_continue = false;
        std::lock_guard<std::mutex> guard(this->lock);
    }
    bool IsRunning() { return this->running; }

   private:
    std::mutex lock;
    std::chrono::milliseconds wait_time;
    MainFunc main_func;
    ErrFunc err_func;
    bool do_continue;
    bool running = false;
};

/*class Pool {
public:

    void Spawn(int wanted) {
        while (this->ref_count != wanted)
            this->Create();
    }
    void SetTimeout(){

    }
private:
    void Create() [
        std::thread t([this](){
            while ()
            // Retrieve task
            std::function task;
            if (!this->tasks.empty()) {
                std::lock_guard<std::mutex> guard(this->lock);
                if (!this->tasks.empty()) { // double check
                    task = this->tasks.front();
                    this->tasks.pop();
                }
            }
            if (task)
                task();
        });
        std::lock_guard<std::mutex> guard(this->lock);
        this->ref_count = 1;
    ]
    std::queue<std::function<void()>> tasks;
    std::mutex lock;
    int ref_count;
};*/

}  // namespace thread
}  // namespace neko
