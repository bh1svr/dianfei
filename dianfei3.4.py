import sys
# 不再导入 secrets，而是导入 random
import random

def main():
    """
    此脚本兼容 Python 3.4 及更高版本。

    - 如果通过命令行提供了一个整数参数，脚本将输出一个从 0 到该整数（不包含）
      范围内的安全随机整数。
    - 如果没有提供命令行参数，脚本将输出一个 0.00 到 100.00 之间的
      安全随机浮点数，并格式化为两位小数。
    """
    try:
        # random.SystemRandom() 创建一个使用 os.urandom() 的实例，
        # 它在 Python 3.4 中可用，功能等同于 secrets 库或 Java 的 SecureRandom。
        csprng = random.SystemRandom()

        if len(sys.argv) > 1:
            # 将第一个参数转换为整数作为上限。
            upper_bound = int(sys.argv[1])
            
            # .randrange(n) 功能与 secrets.randbelow(n) 相同，
            # 生成一个范围在 [0, n) 内的安全随机整数。
            random_integer = csprng.randrange(upper_bound)
            print(random_integer)
        
        else:
            # 如果没有提供参数。
            # 生成一个 0.00 到 100.00 之间的浮点数。
            random_value = csprng.randrange(10001) / 100.0
            
            # --- 这是修改的关键 ---
            # f-string (f'...') 在 Python 3.6 中才引入。
            # 我们使用 str.format() 方法，它在 Python 3.4 和更高版本中都兼容。
            print('{:.2f}'.format(random_value))

    except Exception:
        # 保持与原 Java 代码相同的逻辑，静默处理所有异常。
        pass

if __name__ == "__main__":
    main()
