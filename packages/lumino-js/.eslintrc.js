// このファイルは次のファイルをベースに作成しています。
// Google TypeScript Style Guide: https://github.com/google/gts/blob/main/.eslintrc.json
//
// RMMZ コアスクリプトに合わせていくつかのルールを追加しています。
module.exports = {
    "env": {
        "browser": true,
        "node": true,
    },
    "extends": [
        "eslint:recommended",
        "plugin:node/recommended",
        "plugin:react/recommended",
        "plugin:@typescript-eslint/recommended",
    ],
    "parser": "@typescript-eslint/parser",
    "parserOptions": {
        "ecmaVersion": "latest",
        "sourceType": "module",
    },
    "plugins": ["node", "react", "@typescript-eslint"],
    "settings": {
        "react": {
            // https://github.com/jsx-eslint/eslint-plugin-react#configuration
            "version": "detect",
        },
    },
    "rules": {
        "block-scoped-var": "error",
        "eqeqeq": "error",
        "no-var": "error",
        "prefer-const": "error",
        "eol-last": "error",
        "prefer-arrow-callback": "error",
        "no-trailing-spaces": "error",
        "no-restricted-properties": [
            "error",
            {
                "object": "describe",
                "property": "only",
            },
            {
                "object": "it",
                "property": "only",
            },
        ],

        "node/no-unpublished-import": [
            "error",
            {
                // 以下は devDependencies へのインストールが推奨のため無視する。
                "allowModules": ["electron", "electron-builder", "mocha"],
            },
        ],

        // RMMZ Core script.
        "quotes": ["warn", "double", { "avoidEscape": true }],
        "linebreak-style": ["warn", "unix"],

        // Decorator の次の行にインデントを入れるように要求されてしまうので切っておく。
        // indent は prettier で整形しているため、それに任せる。
        "indent": "off",
    },
    "overrides": [
        {
            "files": ["**/*.ts", "**/*.tsx"],
            "parser": "@typescript-eslint/parser",
            "extends": ["plugin:@typescript-eslint/recommended"],
            "rules": {
                // Google TypeScript Style Guide.
                // https://github.com/google/gts/blob/main/.eslintrc.json
                "@typescript-eslint/no-non-null-assertion": "off",
                "@typescript-eslint/no-use-before-define": "off",
                "@typescript-eslint/no-warning-comments": "off",
                "@typescript-eslint/no-empty-function": "off",
                "@typescript-eslint/no-var-requires": "off",
                "@typescript-eslint/explicit-function-return-type": "off",
                "@typescript-eslint/explicit-module-boundary-types": "off",
                "@typescript-eslint/ban-types": "off",
                "@typescript-eslint/camelcase": "off",
                "node/no-missing-import": "off",
                "node/no-empty-function": "off",
                "node/no-unsupported-features/es-syntax": "off",
                "node/no-missing-require": "off",
                "node/shebang": "off",
                "no-dupe-class-members": "off",
                "require-atomic-updates": "off",

                // ▼ プロジェクト用の設定

                // 空の interface は許可する。
                // RectComponent を作るときに便宜上空の Props を作ることがあるため。
                "@typescript-eslint/no-empty-interface": "off",

                // .tsx に必ず import React を書かないとならないルールは無効化。
                // 「使われてない import がある」指摘と競合してしまうため。
                "react/react-in-jsx-scope": "off",

                // プロト開発中は仮実装のために未使用変数を使うことがある。
                // 使われてない import や変数は VSCode 上でも色分けされるため、警告程度ならそれで十分。
                "@typescript-eslint/no-unused-vars": "off",
                "react/no-unknown-property": "off",

                // 周辺コードとの一貫性のためにあえて型アノテーションを書きたい場合がある。
                "@typescript-eslint/no-inferrable-types": "off",
            },
            "parserOptions": {
                "ecmaVersion": 2018,
                "sourceType": "module",
            },
        },
    ],
};
