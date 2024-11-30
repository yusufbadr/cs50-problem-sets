import os
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # return apology("TODO")
    query = """
    SELECT symbol, number_of_shares
    FROM stocks_owned
    WHERE user_id = ?
    """

    # stocks owned, number of shares owned, current price
    stocks_owned = db.execute(query, session["user_id"])
    total_stocks_value = 0
    for stock_owned in stocks_owned:
        stock = lookup(stock_owned["symbol"])
        stock_owned["price"] = stock["price"]
        total_stocks_value += stock["price"] * stock_owned["number_of_shares"]

    # cash balance
    cash_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # grand total = cash balance + stocks' total value (in index.html)
    grand_total = cash_balance + total_stocks_value
    return render_template("index.html", stocks_owned=stocks_owned, cash_balance=cash_balance, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # """Buy shares of stock"""
    # return apology("TODO")

    if request.method == "POST":
        if not request.form.get("symbol") or not lookup(request.form.get("symbol")):
            return apology("must provide a valid stock name", 400)
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("must provide a valid number")

        if shares <= 0:
            return apology("please provide a positive shares number")

        # actual transaction
        available_balance = db.execute(
            "SELECT cash from users WHERE id = ?",  session["user_id"])[0]["cash"]
        # print(available_balance)

        stock = lookup(request.form.get("symbol"))

        if (stock["price"]*shares <= available_balance):
            today = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            # update transactions table
            db.execute("INSERT INTO transactions (user_id, symbol, number_of_shares, price_per_share, transaction_type, date) VALUES (?, ?, ?, ?, ?, ?)",
                       session["user_id"], stock["symbol"], shares, stock["price"], "BUY", today)

            update_query = """
            UPDATE users
            SET cash = ?
            WHERE id = ?
            """

            # update user's table cash
            db.execute(update_query, available_balance-(stock["price"]*shares), session["user_id"])

            # update stocks_owned
            results = db.execute(
                "SELECT * FROM stocks_owned WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])

            if not results:
                insert_query = """
                INSERT INTO stocks_owned (user_id, symbol, number_of_shares) VALUES (?, ?, ?)
                """
                db.execute(insert_query, session["user_id"], stock["symbol"], shares)
            else:
                current_number_of_shares = results[0]["number_of_shares"]
                update_query = """
                UPDATE stocks_owned
                SET number_of_shares = ?
                WHERE user_id = ? AND symbol = ?
                """
                db.execute(update_query, current_number_of_shares +
                           shares, session["user_id"], stock["symbol"])

            return redirect("/")
        else:
            return apology("not enough funds :(")
    else:
        return render_template("buy.html")


@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    if request.method == "POST":
        if not request.form.get("old_password"):
            return apology("must enter the old password", 400)
        elif not request.form.get("new_password"):
            return apology("must enter the new password", 400)
        elif not request.form.get("confirmation"):
            return apology("must confirm the new password", 400)
        elif request.form.get("confirmation") != request.form.get("new_password"):
            return apology("new password must match confirmation", 400)

        # get the hashed password of the user
        hashed_password = db.execute(
            "SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"]

        if not check_password_hash(hashed_password, request.form.get("old_password")):
            return apology("incorrect old password")

        update_query = """
        UPDATE users
        SET hash = ?
        WHERE id = ?
        """
        db.execute(update_query, generate_password_hash(
            request.form.get("new_password")), session["user_id"])

        session.clear()
        return redirect("/")

    else:
        return render_template("change_password.html")


@app.route("/history")
@login_required
def history():
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("please enter a stock")
        quoted_price = lookup(symbol)
        if (quoted_price is None):
            return apology("please enter a valid stock")
        return render_template("quoted.html", quoted_price=quoted_price)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # return apology("TODO")
    # return render_template("register.html")
    if request.method == "POST":
        # no username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must enter a password", 400)
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("passwords must match", 400)
        else:
            try:
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get(
                    "username"), generate_password_hash(request.form.get("password")))
            except ValueError:
                return apology("username already exists", 400)

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # return apology("TODO")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("no stock chosen")

        stock = lookup(symbol)
        if not stock:
            return apology("incorrect stock symbol")

        query = """
        SELECT symbol, number_of_shares
        FROM stocks_owned
        WHERE user_id = ?
        """
        stocks_owned = db.execute(query, session["user_id"])

        fetched_stock = None
        for stock_owned in stocks_owned:
            if (stock['symbol'] == stock_owned['symbol']):
                fetched_stock = stock_owned

        if fetched_stock is None:
            apology("you do not own that stock")

        # fetched_stock stores the user's specific stock fetched from the database
        # shares is the number of shares the user want to sell
        # stock hold the current stock data

        shares = request.form.get("shares")
        try:
            shares = int(shares)
        except ValueError:
            return apology("Please enter a valid number")

        if (shares <= 0):
            return apology("Please enter a postive number")

        if (shares > fetched_stock['number_of_shares']):
            return apology("You do not own that many shares yet!")

        # if interpreter reaches here then all data entered by the user is valid
        # reduce the number of shares by shares

        cash_gained = shares * stock['price']

        fetched_stock["number_of_shares"] -= shares

        today = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # update transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, number_of_shares, price_per_share, transaction_type, date) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock["symbol"], shares, stock["price"], "SELL", today)

        # update user's table cash
        available_balance = db.execute(
            "SELECT cash from users WHERE id = ?",  session["user_id"])[0]["cash"]

        update_query = """
        UPDATE users
        SET cash = ?
        WHERE id = ?
        """

        db.execute(update_query, available_balance+cash_gained, session["user_id"])

        # update stocks_owned
        results = db.execute(
            "SELECT * FROM stocks_owned WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])

        current_number_of_shares = results[0]["number_of_shares"]

        if (current_number_of_shares - shares > 0):
            update_query = """
            UPDATE stocks_owned
            SET number_of_shares = ?
            WHERE user_id = ? AND symbol = ?
            """
            db.execute(update_query, current_number_of_shares -
                       shares, session["user_id"], stock["symbol"])
        else:
            delete_query = """
            DELETE FROM stocks_owned
            WHERE user_id = ? AND symbol = ?
            """
            db.execute(delete_query, session["user_id"], stock["symbol"])

        return redirect("/")

    else:
        query = """
        SELECT symbol
        FROM stocks_owned
        WHERE user_id = ?
        """

        # stocks owned, number of shares owned
        stocks = db.execute(query, session["user_id"])

        return render_template("sell.html", stocks=stocks)
